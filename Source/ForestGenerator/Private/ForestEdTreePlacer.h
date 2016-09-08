#pragma once

#include "InstancedFoliageActor.h"
#include "InstancedFoliage.h"

class FForestEdTreePlacer
{
public:

	FForestEdTreePlacer(UWorld* InWorld, const FVector &InInitialLocation);

	void Run();

	static FFoliageTraceFilterFunc FilterFunc;

private:

	void MirrorInsert(float X, float Y);

	/**
	 * Insert tree in a cell.
	 *
	 * Cell width and height are equal to Density, so X and Y are enough to define a cell.
	 */
	void InsertInCell(float X, float Y);

	void PlaceTree(float X, float Y);

private:

	UWorld* World;
	FVector InitialLocation;
	float Radius;
	float Density;

};
