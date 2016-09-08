// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ForestGeneratorPrivatePCH.h"
#include "SForestEdit.h"

#include "ForestEdModeSettings.h"

#include "ContentBrowserModule.h"
#include "SNumericEntryBox.h"

#define LOCTEXT_NAMESPACE "ForestEd_Mode"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SForestEdit::Construct(const FArguments& InArgs, TSharedRef<FForestEdModeToolkit> InParentToolkit, FEdMode* InOwningMode)
{
	ForestEditMode = InOwningMode;
	FMargin StandardPadding(0.0f, 5.0f, 0.0f, 4.0f);

	ChildSlot
	[
		SNew(SScrollBox)
		+ SScrollBox::Slot()
		.Padding(0)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
				.Content()
				[
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					.AutoHeight()
					.Padding(6.0f, 0.0f)
					[
						SNew(SVerticalBox)
						+SVerticalBox::Slot()
						.AutoHeight()
						.Padding(StandardPadding)
						[
							SNew(SVerticalBox)
							+SVerticalBox::Slot()
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
								.FillWidth(2.0f)
								.HAlign(HAlign_Left)
								.VAlign(VAlign_Center)
								[
									SNew(SNumericEntryBox<float>)
									.AllowSpin(true)
									.MinSliderValue(1.f)
									.MaxSliderValue(2000.f)
									.MinValue(1.f)
									.MaxValue(2000.f)
									.Value(this, &SForestEdit::GetForestRadius)
									.OnValueChanged(this, &SForestEdit::OnForestRadiusChanged)
								]
							]
							+SVerticalBox::Slot()
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
									.Text(LOCTEXT("ForestEdit_LabelDensity", "Density"))
								]
								+ SHorizontalBox::Slot()
								.FillWidth(2.0f)
								.HAlign(HAlign_Left)
								.VAlign(VAlign_Center)
								[
									SNew(SNumericEntryBox<float>)
									.AllowSpin(true)
									.MinSliderValue(1.f)
									.MaxSliderValue(2000.f)
									.MinValue(1.f)
									.MaxValue(2000.f)
									.Value(this, &SForestEdit::GetForestDensity)
									.OnValueChanged(this, &SForestEdit::OnForestDensityChanged)
								]
							]
						]
						+SVerticalBox::Slot()
						.AutoHeight()
						.Padding(StandardPadding)
						[
							SNew(SSeparator)
							.Orientation(Orient_Horizontal)
						]
						+SVerticalBox::Slot()
						.AutoHeight()
						.Padding(StandardPadding)
						[
							SNew(SVerticalBox)
							+SVerticalBox::Slot()
							.AutoHeight()
							.Padding(StandardPadding)
							[
								SAssignNew(AddTreeBlueprintCombo, SComboButton)
								.ForegroundColor(FLinearColor::White)
								.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
								.OnGetMenuContent(this, &SForestEdit::GetAddTreeBlueprintPicker)
								.ContentPadding(FMargin(1.f))
								.ButtonContent()
								[
									SNew(SHorizontalBox)
									+ SHorizontalBox::Slot()
									.VAlign(VAlign_Center)
									.AutoWidth()
									.Padding(1.f)
									[
										SNew(STextBlock)
										.TextStyle(FEditorStyle::Get(), "ForestEditMode.AddTreeBlueprint.Text")
										.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.9"))
										.Text(FText::FromString(FString(TEXT("\xf067"))) /*fa-plus*/)
									]
									+ SHorizontalBox::Slot()
									.VAlign(VAlign_Center)
									.Padding(1.f)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("AddTreeBlueprintButtonLabel", "Add Tree Blueprint"))
										.TextStyle(FEditorStyle::Get(), "ForestEditMode.AddTreeBlueprint.Text")
									]
								]
							]
							+SVerticalBox::Slot()
							.AutoHeight()
							.Padding(StandardPadding)
							[
								SAssignNew(ListWidgetView, SListView<TSharedPtr<FTreeBlueprintUIInfo>>)
								.ListItemsSource(&FForestEdModeSettings::Get().ForestBlueprints)
								.OnGenerateRow(this, &SForestEdit::OnGenerateRowForList)
							]
						]

					]
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

TOptional<float> SForestEdit::GetForestRadius() const
{
	return FForestEdModeSettings::Get().ForestRadius;
}

void SForestEdit::OnForestRadiusChanged(float InForestRadius)
{
	FForestEdModeSettings::Get().ForestRadius = InForestRadius;
}

TOptional<float> SForestEdit::GetForestDensity() const
{
	return FForestEdModeSettings::Get().ForestDensity;
}

void SForestEdit::OnForestDensityChanged(float InForestDensity)
{
	FForestEdModeSettings::Get().ForestDensity = InForestDensity;
}

TSharedRef<SWidget> SForestEdit::GetAddTreeBlueprintPicker()
{
	// TODO: maybe there is a way in order not to construct the widget from scratch on every call.
	FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	// Configure filter for asset picker
	FAssetPickerConfig Config;
	Config.Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
	Config.InitialAssetViewType = EAssetViewType::List;
	Config.OnAssetSelected = FOnAssetSelected::CreateSP(this, &SForestEdit::OnTreeBlueprintSelected);
	Config.bAllowDragging = false;
	// Don't show stuff in Engine
	Config.Filter.PackagePaths.Add("/Game");
	Config.Filter.bRecursivePaths = true;

	return SNew(SBox)
		.WidthOverride(300.f)
		.HeightOverride(300.f)
		[
			ContentBrowserModule.Get().CreateAssetPicker(Config)
		];
}

void SForestEdit::OnTreeBlueprintSelected(const FAssetData& AssetData)
{
	if (AddTreeBlueprintCombo.IsValid())
	{
		AddTreeBlueprintCombo->SetIsOpen(false);
	}

	TSharedPtr<FTreeBlueprintUIInfo> BlueprintInfo(new FTreeBlueprintUIInfo);
	BlueprintInfo.Get()->Blueprint = Cast<UBlueprint>(AssetData.GetAsset());

	//Adds a new item to the array (do whatever you want with this)
	FForestEdModeSettings::Get().ForestBlueprints.Add(BlueprintInfo);

	//Update the listview
	ListWidgetView->RequestListRefresh();
}

TSharedRef<ITableRow> SForestEdit::OnGenerateRowForList(TSharedPtr<FTreeBlueprintUIInfo> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	const FText BlueprintName = FText::FromString(Item.Get()->Blueprint->GetFriendlyName());
	return
		SNew(STableRow<TSharedPtr<FTreeBlueprintUIInfo>>, OwnerTable)
		.Padding(2.f)
		[
			SNew(STextBlock).Text(BlueprintName)
		];
}

#undef LOCTEXT_NAMESPACE
