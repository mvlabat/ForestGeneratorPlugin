// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ForestGeneratorPrivatePCH.h"
#include "UnrealEd.h"

#include "SForestEdit.h"
#include "Editor/IntroTutorials/Public/IIntroTutorials.h"

#define LOCTEXT_NAMESPACE "FoliageEd_Mode"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SForestEdit::Construct(const FArguments& InArgs)
{
	IIntroTutorials& IntroTutorials = FModuleManager::LoadModuleChecked<IIntroTutorials>(TEXT("IntroTutorials"));

	// Everything (or almost) uses this padding, change it to expand the padding.
	FMargin StandardPadding(6.f, 3.f);
	FMargin StandardLeftPadding(6.f, 3.f, 3.f, 3.f);
	FMargin StandardRightPadding(3.f, 3.f, 6.f, 3.f);

	FSlateFontInfo StandardFont = FEditorStyle::GetFontStyle(TEXT("PropertyWindow.NormalFont"));
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
