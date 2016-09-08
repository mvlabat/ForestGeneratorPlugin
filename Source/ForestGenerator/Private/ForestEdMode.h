#pragma once

#include "EdMode.h"
#include "SharedPointer.h"
#include "Engine/Blueprint.h"

class FEdModeForest : public FEdMode
{
public:
	FEdModeForest();
	~FEdModeForest();

	virtual void Enter() override;

	virtual void Exit() override;

	virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;

	virtual bool MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y) override;

	virtual bool CapturedMouseMove(FEditorViewportClient* InViewportClient, FViewport* InViewport, int32 InMouseX, int32 InMouseY) override;

	bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy *HitProxy, const FViewportClick &Click) override;

private:

	void PlaceTrees(AActor* Actor);

	void ForceRealTimeViewports(bool bEnable, bool bStoreCurrentState);

	void ForestBrushTrace(FEditorViewportClient* ViewportClient, const FVector& InRayOrigin, const FVector& InRayDirection);

	bool bBrushTraceValid;
	FVector BrushLocation;
	FVector BrushTraceDirection;
	UStaticMeshComponent* SphereBrushComponent;

	bool bToolActive;
	bool bAdjustBrushRadius;

};
