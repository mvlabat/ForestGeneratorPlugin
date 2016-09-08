#pragma once

#include "ForestEdMode.h"
#include "ForestEdModeToolkit.h"

#include "AssetData.h"
#include "SListView.h"
#include "STableViewBase.h"
#include "STableRow.h"

struct FFoliageMeshUIInfo;
typedef TSharedPtr<FFoliageMeshUIInfo> FFoliageMeshUIInfoPtr; //should match typedef in FoliageEdMode.h

class SForestEdit : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SForestEdit) {}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, TSharedRef<FForestEdModeToolkit> InParentToolkit, FEdMode* InOwningMode);

	FEdMode* GetEditorMode() const;

protected:

	TOptional<float> GetForestRadius() const;
	void OnForestRadiusChanged(float InForestRadius);

	TOptional<float> GetForestDensity() const;
	void OnForestDensityChanged(float InForestDensity);

	TSharedRef<SWidget> GetAddTreeBlueprintPicker();
	void OnTreeBlueprintSelected(const FAssetData& AssetData);
	FReply OnDeleteAllClicked();
	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FTreeBlueprintUIInfo> Item, const TSharedRef<STableViewBase>& OwnerTable);


private:
	FEdMode* ForestEditMode;
	TSharedPtr<SComboButton> AddTreeBlueprintCombo;
	TSharedPtr<SListView<TSharedPtr<FTreeBlueprintUIInfo>>> ListWidgetView;
};
