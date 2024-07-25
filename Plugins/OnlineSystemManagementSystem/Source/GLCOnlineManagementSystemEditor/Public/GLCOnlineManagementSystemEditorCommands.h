// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "GLCOnlineManagementSystemEditorStyle.h"

class FGLCOnlineManagementSystemEditorCommands : public TCommands<FGLCOnlineManagementSystemEditorCommands>
{
public:

	FGLCOnlineManagementSystemEditorCommands()
		: TCommands<FGLCOnlineManagementSystemEditorCommands>(TEXT("GLCOnlineManagementSystemEditor"), NSLOCTEXT("Contexts", "GLCOnlineManagementSystemEditor", "GLCOnlineManagementSystemEditor Plugin"), NAME_None, FGLCOnlineManagementSystemEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};
