// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ForestGeneratorPrivatePCH.h"
#include "ForestEdModeToolkit.h"

#include "SForestEdit.h"

#define LOCTEXT_NAMESPACE "FoliageEditMode"


FForestEdModeToolkit::FForestEdModeToolkit(FEdMode* InOwningMode)
	: ForestEdMode(InOwningMode)
{

}

void FForestEdModeToolkit::RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager)
{

}

void FForestEdModeToolkit::UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager)
{

}

void FForestEdModeToolkit::Init(const TSharedPtr< class IToolkitHost >& InitToolkitHost)
{
	ForestEdWidget = SNew(SForestEdit, SharedThis(this), ForestEdMode);

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
	return ForestEdMode;
}

TSharedPtr<SWidget> FForestEdModeToolkit::GetInlineContent() const
{
	return ForestEdWidget;
}

#undef LOCTEXT_NAMESPACE
