// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ForestGeneratorPrivatePCH.h"
#include "SForestEdit.h"

#include "SNumericEntryBox.h"

#define LOCTEXT_NAMESPACE "ForestEd_Mode"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SForestEdit::Construct(const FArguments& InArgs, TSharedRef<FForestEdModeToolkit> InParentToolkit, FEdMode* InOwningMode)
{
	ForestEditMode = InOwningMode;
	FMargin StandardPadding(0.0f, 5.0f, 0.0f, 4.0f);

	//const float BrushRadius = ForestEditMode->GetBrushRadiiDefault();

	//float MinBrushSliderRadius, MaxBrushSliderRadius;
	//ForestEditMode->GetBrushRadiiSliderLimits(MinBrushSliderRadius, MaxBrushSliderRadius);

	ChildSlot
	[
		SNew(SScrollBox)
			+ SScrollBox::Slot()
			.Padding(0.0f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(StandardPadding)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("ForestEdit_LabelRadius", "Radius"))
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Center)
					[
						SNew(SNumericEntryBox<float>)
						.AllowSpin(true)
						.MinSliderValue(1.f)
						.MaxSliderValue(256.f)
						.MinValue(0.1f)
						.MaxValue(250000.f)
						//.Value(this, 100.f)
						//.OnValueChanged(this )
					]
				]
			]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FEdMode* SForestEdit::GetEditorMode() const
{
	return ForestEditMode;
}



#undef LOCTEXT_NAMESPACE
