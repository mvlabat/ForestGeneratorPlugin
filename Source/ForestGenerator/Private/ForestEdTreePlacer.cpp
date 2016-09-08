#include "ForestGeneratorPrivatePCH.h"
#include "ForestEdTreePlacer.h"

#include "ForestEdModeSettings.h"

#include "Math/UnrealMathUtility.h"

FForestEdTreePlacer::FForestEdTreePlacer(UWorld* InWorld, const FVector &InInitialLocation)
	: World(InWorld)
	, InitialLocation(InInitialLocation)
	, Radius(FForestEdModeSettings::Get().ForestRadius)
	, Density(FForestEdModeSettings::Get().ForestDensity)
{ }

void FForestEdTreePlacer::Run()
{
	if (FForestEdModeSettings::Get().ForestBlueprints.Num() == 0)
		return;

	float X = Density;
	float Y = X; // just in order to enter the loop
	while (X <= Y)
	{
		Y = FMath::RoundToZero(FMath::Sqrt(Radius * Radius - X * X));
		MirrorInsert(X, X);
		for (float i = X + Density; i <= Y; i += Density)
		{
			MirrorInsert(X, i);
			MirrorInsert(i, X);
		}
		X += Density;
	}
}

void FForestEdTreePlacer::MirrorInsert(float X, float Y)
{
	InsertInCell(InitialLocation.X + X - Density, InitialLocation.Y + Y - Density);
	InsertInCell(InitialLocation.X - X, InitialLocation.Y + Y - Density);
	InsertInCell(InitialLocation.X + X - Density, InitialLocation.Y - Y);
	InsertInCell(InitialLocation.X - X, InitialLocation.Y - Y);
}

void FForestEdTreePlacer::InsertInCell(float X, float Y)
{
	X = FMath::RandRange(X, X + Density);
	Y = FMath::RandRange(Y, Y + Density);
	PlaceTree(X, Y);
}

FFoliageTraceFilterFunc FForestEdTreePlacer::FilterFunc([](const UPrimitiveComponent* Component) -> bool
{
	return Component && Component->IsA(ULandscapeHeightfieldCollisionComponent::StaticClass());
});

void FForestEdTreePlacer::PlaceTree(float X, float Y)
{
	const FVector Location = FVector(X, Y, 0);

	const FVector HaldZMax = HALF_WORLD_MAX * FVector(0, 0, 1);
	const FVector TraceStart(Location - HaldZMax);
	const FVector TraceEnd(Location + HaldZMax);

	FHitResult Hit;

	static FName NAME_ForestBrush = FName(TEXT("ForestBrush"));
	const int BlueprintsNum = FForestEdModeSettings::Get().ForestBlueprints.Num();
	const int BlueprintIndex = FMath::RandRange(0, BlueprintsNum - 1);
	UClass* ActorClass = Cast<UClass>(FForestEdModeSettings::Get().ForestBlueprints[BlueprintIndex].Get()->Blueprint->GeneratedClass);
	if (AInstancedFoliageActor::FoliageTrace(World, Hit, FDesiredFoliageInstance(TraceStart, TraceEnd), NAME_ForestBrush, false, FForestEdTreePlacer::FilterFunc))
	{
		World->SpawnActor(ActorClass, &Hit.Location);
	}
}
