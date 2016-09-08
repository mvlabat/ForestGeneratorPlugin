#include "ForestGeneratorPrivatePCH.h"
#include "ForestEdMode.h"

#include "ForestEdModeSettings.h"
#include "ForestEdModeToolkit.h"
#include "ForestEdTreePlacer.h"

#include "EditorModes.h"
#include "ToolkitManager.h"
#include "Components/BrushComponent.h"
#include "LevelEditor.h"
#include "SLevelViewport.h"
#include "LandscapeHeightfieldCollisionComponent.h"
#include "ProceduralFoliageBlockingVolume.h"

#define LOCTEXT_NAMESPACE "FoliageEdMode"

FEdModeForest::FEdModeForest()
	: FEdMode()
	, bToolActive(false)
	, bAdjustBrushRadius(false)
{
	UMaterial* BrushMaterial = nullptr;
	UStaticMesh* StaticMesh = nullptr;
	if (!IsRunningCommandlet())
	{
		BrushMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Engine/EditorLandscapeResources/FoliageBrushSphereMaterial.FoliageBrushSphereMaterial"), nullptr, LOAD_None, nullptr);
		StaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/EngineMeshes/Sphere.Sphere"), nullptr, LOAD_None, nullptr);
	}

	SphereBrushComponent = NewObject<UStaticMeshComponent>(GetTransientPackage(), TEXT("SphereBrushComponent"));
	SphereBrushComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	SphereBrushComponent->SetCollisionObjectType(ECC_WorldDynamic);
	SphereBrushComponent->StaticMesh = StaticMesh;
	SphereBrushComponent->OverrideMaterials.Add(BrushMaterial);
	SphereBrushComponent->SetAbsolute(true, true, true);
	SphereBrushComponent->CastShadow = false;

	bBrushTraceValid = false;
	BrushLocation = FVector::ZeroVector;
}

FEdModeForest::~FEdModeForest()
{

}

void FEdModeForest::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid())
	{
		Toolkit = MakeShareable(new FForestEdModeToolkit(this));
		Toolkit->Init(Owner->GetToolkitHost());
	}

	ForceRealTimeViewports(true, true);
}

void FEdModeForest::Exit()
{
	FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
	Toolkit.Reset();

	if (SphereBrushComponent->IsRegistered()) {
		// TODO: find out why it crashes the editor
		SphereBrushComponent->UnregisterComponent();
	}

	ForceRealTimeViewports(false, false);

	FEdMode::Exit();
}

void FEdModeForest::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{
	FEdMode::Tick(ViewportClient, DeltaTime);

	if (bBrushTraceValid) {
		// Scale adjustment is due to default sphere SM size.
		FTransform BrushTransform = FTransform(FQuat::Identity, BrushLocation, FVector(FForestEdModeSettings::Get().ForestRadius * 0.00625f));
		SphereBrushComponent->SetRelativeTransform(BrushTransform);

		if (!SphereBrushComponent->IsRegistered())
		{
			SphereBrushComponent->RegisterComponentWithWorld(ViewportClient->GetWorld());
		}
	}
	else
	{
		if (SphereBrushComponent->IsRegistered())
		{
			SphereBrushComponent->UnregisterComponent();
		}
	}
}

bool FEdModeForest::MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 MouseX, int32 MouseY)
{
	// Compute a world space ray from the screen space mouse coordinates
	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
		ViewportClient->Viewport,
		ViewportClient->GetScene(),
		ViewportClient->EngineShowFlags)
		.SetRealtimeUpdate(ViewportClient->IsRealtime()));

	FSceneView* View = ViewportClient->CalcSceneView(&ViewFamily);
	FViewportCursorLocation MouseViewportRay(View, ViewportClient, MouseX, MouseY);
	BrushTraceDirection = MouseViewportRay.GetDirection();

	FVector BrushTraceStart = MouseViewportRay.GetOrigin();
	if (ViewportClient->IsOrtho())
	{
		BrushTraceStart += -WORLD_MAX * BrushTraceDirection;
	}

	ForestBrushTrace(ViewportClient, BrushTraceStart, BrushTraceDirection);

	return false;
}

bool FEdModeForest::CapturedMouseMove(FEditorViewportClient* InViewportClient, FViewport* InViewport, int32 InMouseX, int32 InMouseY)
{
	return MouseMove(InViewportClient, InViewport, InMouseX, InMouseY);
}

bool FEdModeForest::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy *HitProxy, const FViewportClick &Click)
{

	if (HitProxy && HitProxy->IsA(HActor::StaticGetType()))
	{
		GEditor->BeginTransaction(NSLOCTEXT("UnrealEd", "ForestMode_EditTransaction", "Forest Adding"));

		PlaceTrees(((HActor*)HitProxy)->Actor);

		GEditor->EndTransaction();
	}

	return true;

	return FEdMode::HandleClick(InViewportClient, HitProxy, Click);
}

void FEdModeForest::PlaceTrees(AActor* Actor)
{
	FForestEdTreePlacer TreePlacer(Actor->GetWorld(), BrushLocation);
	TreePlacer.Run();
}

void FEdModeForest::ForceRealTimeViewports(const bool bEnable, const bool bStoreCurrentState)
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<ILevelViewport> ViewportWindow = LevelEditorModule.GetFirstActiveViewport();
	if (ViewportWindow.IsValid())
	{
		FEditorViewportClient &Viewport = ViewportWindow->GetLevelViewportClient();
		if (Viewport.IsPerspective())
		{
			if (bEnable)
			{
				Viewport.SetRealtime(bEnable, bStoreCurrentState);
			}
			else
			{
				const bool bAllowDisable = true;
				Viewport.RestoreRealtime(bAllowDisable);
			}

		}
	}
}

void FEdModeForest::ForestBrushTrace(FEditorViewportClient* ViewportClient, const FVector& InRayOrigin, const FVector& InRayDirection)
{
	bBrushTraceValid = false;
	if (!ViewportClient->IsMovingCamera() && ViewportClient->IsVisible())
	{
		const FVector TraceStart(InRayOrigin);
		const FVector TraceEnd(InRayOrigin + InRayDirection * HALF_WORLD_MAX);

		FHitResult Hit;
		UWorld* World = ViewportClient->GetWorld();
		static FName NAME_ForestBrush = FName(TEXT("ForestBrush"));

		if (AInstancedFoliageActor::FoliageTrace(World, Hit, FDesiredFoliageInstance(TraceStart, TraceEnd), NAME_ForestBrush, false, FForestEdTreePlacer::FilterFunc))
		{
			UPrimitiveComponent* PrimComp = Hit.Component.Get();
			//if (CanPaint(PrimComp->GetComponentLevel()))
			{
				if (!bAdjustBrushRadius)
				{
					// Adjust the brush location
					BrushLocation = Hit.Location;
				}

				// Still want to draw the brush when resizing
				bBrushTraceValid = true;
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
