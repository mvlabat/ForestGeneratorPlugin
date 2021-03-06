#pragma once

#include "Toolkits/BaseToolkit.h"
#include "SharedPointer.h"

class FEdModeForest;

/**
* Public interface to Foliage Edit mode.
*/
class FForestEdModeToolkit : public FModeToolkit
{
public:
	FForestEdModeToolkit(FEdMode *InOwningMode);

	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

	/** Initializes the foliage mode toolkit */
	virtual void Init(const TSharedPtr< class IToolkitHost >& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override;

private:
	FEdMode *ForestEdMode;
	TSharedPtr< class SForestEdit > ForestEdWidget;
};
