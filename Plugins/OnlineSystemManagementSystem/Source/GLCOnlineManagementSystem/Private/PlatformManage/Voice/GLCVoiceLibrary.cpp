// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformManage/Voice/GLCVoiceLibrary.h"
#include <PlatformManage/Voice/Core/OnlineVoiceManage.h>
#include "GameFramework/OnlineReplStructs.h"
#include "PlatformManage/Voice/UI/Core/UI_VoiceChatMain.h"
#include <PlatformManage/Common/GLCPlatformManageLibrary.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

#include "Common/Components/GLCOnlineManageComponent.h"
#include "PlatformManage/Voice/UI/Elements/UI_VoiceChatInfo.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformManage/Voice/UI/Core/UI_VoiceChatMain.h"
#include "PlatformManage/Voice/UI/Elements/UI_VoiceChatInfo.h"


TSubclassOf<UUI_VoiceChatMain> UGLCVoiceLibrary::VoiceChatMainClass = nullptr;
TSubclassOf<UUI_VoiceChatInfo> UGLCVoiceLibrary::VoiceChatInfoClass = nullptr;

UGLCVoiceLibrary::UGLCVoiceLibrary()
{
	ConstructorHelpers::FClassFinder<UUI_VoiceChatMain> VoiceChatMainFinder(TEXT("/GLCOnlineManagementSystem/ManagementSystem/VoiceChat/Core/BP_UI_VoiceChatMain"));
	VoiceChatMainClass = VoiceChatMainFinder.Class;

	ConstructorHelpers::FClassFinder<UUI_VoiceChatInfo> VoiceChatInfoClassFinder(TEXT("/GLCOnlineManagementSystem/ManagementSystem/VoiceChat/Element/BP_UI_VoiceChatInfo"));
	VoiceChatInfoClass = VoiceChatInfoClassFinder.Class;
}

void UGLCVoiceLibrary::StartNetworkedVoice()
{
	UOnlineVoiceManage::Get()->ToggleSpeaking(true);
}

void UGLCVoiceLibrary::StopNetworkedVoice()
{
	UOnlineVoiceManage::Get()->ToggleSpeaking(false);
}

void UGLCVoiceLibrary::MutePlayer(const FUniqueNetIdRepl& PlayerNetId)
{
	UOnlineVoiceManage::Get()->GameplayMutePlayer(PlayerNetId);
}

void UGLCVoiceLibrary::UnmutePlayer(const FUniqueNetIdRepl& PlayerNetId)
{
	UOnlineVoiceManage::Get()->GameplayUnmutePlayer(PlayerNetId);
}

void UGLCVoiceLibrary::SetIsMutePlayer(const FUniqueNetIdRepl& PlayerNetId, bool bIsMute)
{
	if (bIsMute)
	{
		MutePlayer(PlayerNetId);
	}
	else
	{
		UnmutePlayer(PlayerNetId);
	}
}

bool UGLCVoiceLibrary::IsPlayerMuted(const FUniqueNetIdRepl& PlayerId)
{
	return UOnlineVoiceManage::Get()->IsPlayerMuted(PlayerId);
}

UUI_VoiceChatMain* UGLCVoiceLibrary::CreateVoiceChatWidget(bool bInputUIOnly /*= true*/, 
	bool bShowMouse /*= true*/)
{
	if (APlayerController* InPC = UGLCPlatformManageLibrary::GetPlayerController())
	{
		if (UUI_VoiceChatMain* InMain = CreateWidget<UUI_VoiceChatMain>(InPC->GetWorld(), VoiceChatMainClass))
		{
			if (bInputUIOnly)
			{
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(InPC);
			}
			InPC->SetShowMouseCursor(bShowMouse);

			return InMain;
		}
	}

	return nullptr;
}

void UGLCVoiceLibrary::ShowVoiceChatWidget()
{
	if (UGLCOnlineManageComponent* InCom = UGLCPlatformManageLibrary::GetGLCOnlineManagerComponent())
	{
		if (APlayerController* InPlayerController = UGLCPlatformManageLibrary::GetPlayerController())
		{
			if (UUI_VoiceChatMain* InChatMain = InCom->GetVoiceChatMain())
			{
				if (IsValid(InChatMain))
				{
					InChatMain->UpdatePlayerList();
					InChatMain->PlayVoiceChatInAnim();
				}
				else
				{
					if (UUI_VoiceChatMain* InCreateChatMain = CreateVoiceChatWidget(false))
					{
						InCreateChatMain->SetupUI();

						InCom->SetVoiceChatMain(InCreateChatMain);
					}
				}
			}
			else
			{
				if (UUI_VoiceChatMain* InCreateChatMain = CreateVoiceChatWidget(false))
				{
					InCreateChatMain->SetupUI();
					InCom->SetVoiceChatMain(InCreateChatMain);
				}
			}
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(InPlayerController);
			InPlayerController->SetShowMouseCursor(true);
		}
	}
}
