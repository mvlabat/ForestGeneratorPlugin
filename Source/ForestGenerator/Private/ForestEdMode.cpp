#include "ForestGeneratorPrivatePCH.h"
#include "ForestEdMode.h"

#include "ForestEdModeToolkit.h"

#include "EditorModes.h"
#include "ToolkitManager.h"

FForestEdModeSettings FForestEdModeSettings::StaticForestEdModeSettings;

FEdModeForest::FEdModeForest()
	: FEdMode()
{
	
}

FEdModeForest::~FEdModeForest()
{

}

void FEdModeForest::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid())
	{
		Toolkit = MakeShareable(new FForestEdModeToolkit(this));
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FEdModeForest::Exit()
{
	FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
	Toolkit.Reset();

	FEdMode::Exit();
}
