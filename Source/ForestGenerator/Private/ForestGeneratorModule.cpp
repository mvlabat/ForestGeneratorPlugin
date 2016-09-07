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
	FEditorModeRegistry::Get().UnregisterMode(TEXT("EM_Forest"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FForestGeneratorModule, ForestGenerator)
