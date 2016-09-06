#pragma once

#include "ForestEdModeToolkit.h"

struct FFoliageMeshUIInfo;
typedef TSharedPtr<FFoliageMeshUIInfo> FFoliageMeshUIInfoPtr; //should match typedef in FoliageEdMode.h

class SForestEdit : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SForestEdit) {}
	SLATE_END_ARGS()

public:
	/** SCompoundWidget functions */
	void Construct(const FArguments& InArgs, TSharedRef<FForestEdModeToolkit> InParentToolkit, FEdMode* InOwningMode);

	FEdMode* GetEditorMode() const;

private:
	FEdMode* ForestEditMode;
};
