// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ForestGeneratorPrivatePCH.h"
#include "UnrealEd.h"
#include "ForestEdModeToolkit.h"
#include "SForestEdit.h"

#define LOCTEXT_NAMESPACE "FoliageEditMode"

void FForestEdModeToolkit::RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager)
{

}

void FForestEdModeToolkit::UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager)
{

}

void FForestEdModeToolkit::Init(const TSharedPtr< class IToolkitHost >& InitToolkitHost)
{
	FoliageEdWidget = SNew(SForestEdit);

	FModeToolkit::Init(InitToolkitHost);
}

FName FForestEdModeToolkit::GetToolkitFName() const
{
	return FName("ForestEditMode");
}

FText FForestEdModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("ToolkitName", "Forest Edit Mode");
}

class FEdMode* FForestEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(TEXT("EM_Forest"));
}

TSharedPtr<SWidget> FForestEdModeToolkit::GetInlineContent() const
{
	return FoliageEdWidget;
}

void FForestEdModeToolkit::RefreshFullList()
{
	//FoliageEdWidget->RefreshFullList();
}

void FForestEdModeToolkit::NotifyFoliageTypeMeshChanged(class UFoliageType* FoliageType)
{
	//FoliageEdWidget->NotifyFoliageTypeMeshChanged(FoliageType);
}

#undef LOCTEXT_NAMESPACE
