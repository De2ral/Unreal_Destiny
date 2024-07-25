// Copyright Epic Games, Inc. All Rights Reserved.

#include "GLCOnlineManagementSystemEditorCommands.h"

#define LOCTEXT_NAMESPACE "FGLCOnlineManagementSystemEditorModule"

void FGLCOnlineManagementSystemEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "GLCOnlineManagementSystemEditor", "Bring up GLCOnlineManagementSystemEditor window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
