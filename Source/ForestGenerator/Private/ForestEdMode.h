#pragma once

#include "EdMode.h"
#include "SharedPointer.h"
#include "Engine/Blueprint.h"

struct FTreeBlueprintUIInfo {
	UBlueprint* Blueprint;
	float TreeRadius; // TODO: is not used currently
};

/**
 * ForestEdMode Settings
 */
class FForestEdModeSettings {
public:

	static FForestEdModeSettings& Get()
	{
		return StaticForestEdModeSettings;
	}

protected:

	static FForestEdModeSettings StaticForestEdModeSettings;

public:

	FForestEdModeSettings()
	: ForestRadius(500)
	, ForestDensity(20)
	{ }

public:

	float ForestRadius;

	float ForestDensity;

	TArray<TSharedPtr<FTreeBlueprintUIInfo>> ForestBlueprints;

};

class FEdModeForest : public FEdMode
{
public:
	FEdModeForest();
	~FEdModeForest();

	virtual void Enter() override;

	virtual void Exit() override;

};
