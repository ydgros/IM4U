// Copyright Epic Games, Inc. All Rights Reserved.
#include "MMDCameraImporterStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMmdCameraImporterStyle::StyleInstance = nullptr;

void FMmdCameraImporterStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMmdCameraImporterStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMmdCameraImporterStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MMDCameraImporterStyle"));
	return StyleSetName;
}


TSharedRef<FSlateStyleSet> FMmdCameraImporterStyle::Create()
{
	// No icon artwork is registered: the Import VMD tool bar entry is a plain text button.
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MMDCameraImporterStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("IM4U")->GetBaseDir() / TEXT("Resources"));

	return Style;
}

void FMmdCameraImporterStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMmdCameraImporterStyle::Get()
{
	return *StyleInstance;
}
