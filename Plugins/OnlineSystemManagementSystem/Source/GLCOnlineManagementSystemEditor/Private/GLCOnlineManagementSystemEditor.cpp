// Copyright Epic Games, Inc. All Rights Reserved.

#include "GLCOnlineManagementSystemEditor.h"
#include "GLCOnlineManagementSystemEditorStyle.h"
#include "GLCOnlineManagementSystemEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Slate/SInitPannel.h"
#include <Manage/OnlineSubsystemEditorManage.h>

static const FName GLCOnlineManagementSystemEditorTabName("GLCOnlineManagementSystemEditor");

#define LOCTEXT_NAMESPACE "FGLCOnlineManagementSystemEditorModule"

void FGLCOnlineManagementSystemEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FGLCOnlineManagementSystemEditorStyle::Initialize();
	FGLCOnlineManagementSystemEditorStyle::ReloadTextures();

	FGLCOnlineManagementSystemEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FGLCOnlineManagementSystemEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FGLCOnlineManagementSystemEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGLCOnlineManagementSystemEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(GLCOnlineManagementSystemEditorTabName, FOnSpawnTab::CreateRaw(this, &FGLCOnlineManagementSystemEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FGLCOnlineManagementSystemEditorTabTitle", "GLCOnlineManagementSystemEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FGLCOnlineManagementSystemEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FGLCOnlineManagementSystemEditorStyle::Shutdown();

	FGLCOnlineManagementSystemEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GLCOnlineManagementSystemEditorTabName);

	EPlatformState PlatformState = FOnlineSubsystemEditorManage::Get()->GetPlatformState();
	switch (PlatformState)
	{
	case EPlatformState::Null:
		break;
	case EPlatformState::EOSForSteam:
		FOnlineSubsystemEditorManage::Get()->InitEOSForSteam_Exe();
		break;
	case EPlatformState::EOSForEpic:
		FOnlineSubsystemEditorManage::Get()->InitEOSForEpic_Exe();
		break;
	default:
		break;
	}
	FOnlineSubsystemEditorManage::Destroy();
}

TSharedRef<SDockTab> FGLCOnlineManagementSystemEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FGLCOnlineManagementSystemEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("GLCOnlineManagementSystemEditor.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.VAlign(EVerticalAlignment::VAlign_Fill)
			[
				SNew(SSInitPannel)
			]
		];
}

void FGLCOnlineManagementSystemEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(GLCOnlineManagementSystemEditorTabName);
}

void FGLCOnlineManagementSystemEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FGLCOnlineManagementSystemEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FGLCOnlineManagementSystemEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGLCOnlineManagementSystemEditorModule, GLCOnlineManagementSystemEditor)
