// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformManage/Voice/UI/Core/UI_VoiceChatMain.h"

#include "PlatformManage/Voice/UI/UI_VoiceChatWindow.h"

void UUI_VoiceChatMain::UpdatePlayerList()
{
	UI_VoiceChatWindow->UpdateVoiceChatWindow();
}

void UUI_VoiceChatMain::SetupUI()
{
	UI_VoiceChatWindow->SetFather(this);

	AddToViewport(10);
	UpdatePlayerList();
	PlayVoiceChatInAnim();
}

