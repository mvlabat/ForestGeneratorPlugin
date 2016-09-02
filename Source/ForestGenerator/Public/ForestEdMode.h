#pragma once

#include "EdMode.h"
#include "SharedPointer.h"

class FEdModeForest : public FEdMode
{
public:
	FEdModeForest();
	~FEdModeForest();

	virtual void Enter() override;

	virtual void Exit() override;

protected:
	/** Editor Mode Toolkit that is associated with this toolkit mode */
	TSharedPtr<class FModeToolkit> Toolkit;

	/** Pointer back to the mode tools that we are registered with */
	FEditorModeTools* Owner;
};
