#include "PlatformManage/Voice/Core/OnlineVoiceManage.h"
#include "Runtime/Launch/Resources/Version.h"
#include <OnlineSubsystem.h>
#include <PlatformManage/Common/GLCPlatformManageLibrary.h>
#include "VoiceChat.h"
#include "Common/GLCGameInstance.h"
#include "Common/Components/GLCOnlineManageComponent.h"
#include "PlatformManage/Voice/Core/GLCOnlineVoiceObject.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/OnlineReplStructs.h"
#include "GameFramework/PlayerState.h"
#include "Engine/GameInstance.h"
#if ENGINE_MAJOR_VERSION >= 5
#include "IOnlineSubsystemEOS.h"
#endif


UOnlineVoiceManage::UOnlineVoiceManage()
{	
	/*OnlineVoice = IOnlineSubsystem::Get()->GetVoiceInterface();

	if (OnlineVoice)
	{
		OnlineVoice->AddOnPlayerTalkingStateChangedDelegate_Handle(FOnPlayerTalkingStateChangedDelegate::CreateWeakLambda(this,
		[&](FUniqueNetIdRef InNetID, bool bIsTalking)
		{
			if (UGLCOnlineManageComponent* InManageCom = UGLCPlatformManageLibrary::GetGLCOnlineManagerComponent())
			{
				if (UOnlineVoiceObject* InVoiceObject = InManageCom->GLCOnlineVoiceObject.GetDefaultObject())
				{
					InVoiceObject->OnPlayerTalkingStateChange(InNetID, bIsTalking);
				}
			}
		}));
	}*/
}

UOnlineVoiceManage* UOnlineVoiceManage::Get()
{
	if(UGameInstance* InGI = UGLCPlatformManageLibrary::GetGameInstance())
	{
		return InGI->GetSubsystem<UOnlineVoiceManage>();
	}
	return nullptr;
}

void UOnlineVoiceManage::ToggleSpeaking(bool bStart)
{
#if ENGINE_MAJOR_VERSION >= 5
	if(IOnlineSubsystem* system = IOnlineSubsystem::Get())
	{
		if(IOnlineSubsystemEOS* EOSp = static_cast<IOnlineSubsystemEOS*>(system))
		{
			FUniqueNetIdRepl ID;
			UGLCPlatformManageLibrary::GetUniqueNetID(ID);
			checkf(ID.IsValid(),TEXT("ID is not valid"));
			if(!ID.IsValid()) return;
			
			if(IVoiceChatUser* VoiceChatUser = EOSp->GetVoiceChatUserInterface(*ID))
			{
				VoiceChatUser->SetAudioInputDeviceMuted(!bStart);
				return;
			}
		}
	}
#endif
	
	if (APlayerController* InPlayerController = UGLCPlatformManageLibrary::GetPlayerController())
	{
		if(bStart)
		{
			InPlayerController->StartTalking();
			OnPlayerTalkingStateChange(InPlayerController,true);
		}
		else
		{
			InPlayerController->StopTalking();
			OnPlayerTalkingStateChange(InPlayerController, false);
		}
	}
}

void UOnlineVoiceManage::GameplayMutePlayer(const FUniqueNetIdRepl& PlayerNetId)
{
	if (APlayerController* InPlayerController = UGLCPlatformManageLibrary::GetPlayerController())
	{
		InPlayerController->GameplayMutePlayer(PlayerNetId);
	}
}

void UOnlineVoiceManage::GameplayUnmutePlayer(const FUniqueNetIdRepl& PlayerNetId)
{
	if (APlayerController* InPlayerController = UGLCPlatformManageLibrary::GetPlayerController())
	{
		InPlayerController->GameplayUnmutePlayer(PlayerNetId);
	}
}

bool UOnlineVoiceManage::IsPlayerMuted(const FUniqueNetIdRepl& PlayerId)
{
	if (APlayerController* InPlayerController = UGLCPlatformManageLibrary::GetPlayerController())
	{
		return InPlayerController->IsPlayerMuted(*PlayerId);
	}
	return false;
}

void UOnlineVoiceManage::OnPlayerTalkingStateChange(APlayerController* InPlayerController, bool bIsTalking)
{
	if (IsValid(InPlayerController))
	{
		if (UGLCGameInstance* Gi = UGLCPlatformManageLibrary::GetGLCGameInstance())
		{
			if (Gi->IsInSession() == 0)return;
		}
		if (UGLCOnlineManageComponent* InManageCom = UGLCPlatformManageLibrary::GetGLCOnlineManagerComponent())
		{
			if (InPlayerController->PlayerState)
			{
				InManageCom->Server_OnPlayerTalkingStateChange(InPlayerController->PlayerState->GetUniqueId(),
					bIsTalking);
			}
		}
	}
}
