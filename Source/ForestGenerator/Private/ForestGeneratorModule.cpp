// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ForestGeneratorPrivatePCH.h"
#include "ForestEdMode.h"
#include "../Public/ForestGeneratorModule.h"

#define LOCTEXT_NAMESPACE "FForestGeneratorModule"

void FForestGeneratorModule::StartupModule()
{
	FEditorModeRegistry::Get().RegisterMode<FEdModeForest>(
		TEXT("EM_Forest"),
		NSLOCTEXT("EditorModes", "ForestMode", "Forest"),
		FSlateIcon(),
		true, 400
		);
}

void FForestGeneratorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FForestGeneratorModule, ForestGenerator)
