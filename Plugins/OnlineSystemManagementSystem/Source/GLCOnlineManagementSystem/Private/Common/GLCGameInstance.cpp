// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/GLCGameInstance.h"

#include "Common/Type/GLCCommonType.h"
#include <PlatformManage/Common/GLCPlatformManageLibrary.h>
#include <PlatformManage/Voice/GLCVoiceLibrary.h>
#include "GameFramework/OnlineReplStructs.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Common/Components/GLCOnlineManageComponent.h"
#include "PlatformManage/Voice/Core/GLCOnlineVoiceObject.h"
#include "PlatformManage/Common/PlatformManageType.h"
#include "PlatformManage/OnlineSessions/Common/GLCOnlineSessionsLibrary.h"


UGLCGameInstance::UGLCGameInstance()
	:bIsInSession(false)
{
	
}

void UGLCGameInstance::GetAllPlayerInfo(TArray<FGLCPlayerInfo>& OutInfos, bool bGetSelfInfo)
{
	if (UGLCGameInstance* InGI = UGLCPlatformManageLibrary::GetGLCGameInstance())
	{
		TArray<FGLCPlayerInfo> NewInfos;
		UGLCPlatformManageLibrary::GetAllPlayerInfo_OnlyPNAndNetID(NewInfos);

		for (auto& Temp : NewInfos)
		{
			FGLCPlayerInfo NewInfo;
			NewInfo.NetID = Temp.NetID;
			NewInfo.PlayerName = Temp.PlayerName;
			NewInfo.bIsMute = UGLCVoiceLibrary::IsPlayerMuted(Temp.NetID);

			int32 FIndex = PlayerVoiceInfos.Find(Temp);
			if (FIndex != INDEX_NONE)
			{
				PlayerVoiceInfos[FIndex].bIsMute = NewInfo.bIsMute;
			}
			else
			{
				PlayerVoiceInfos.Add(NewInfo);
			}
		}
		FGLCPlayerInfo PlayerSelfInfo;
		for (FGLCPlayerInfo& Temp : PlayerVoiceInfos)
		{
			int32 FIndex = NewInfos.Find(Temp);
			if (FIndex == INDEX_NONE)
			{
				PlayerVoiceInfos.RemoveSingle(Temp);
			}

			if (!bGetSelfInfo && !PlayerSelfInfo.IsValid())
			{
				if (APlayerController* InPC = UGLCPlatformManageLibrary::GetPlayerController())
				{
					if (APlayerState* InPS = InPC->PlayerState)
					{
						if (Temp.NetID == InPS->GetUniqueId())
						{
							PlayerSelfInfo = Temp;
						}
					}
				}
			}
		}

		OutInfos = PlayerVoiceInfos;
		if (PlayerSelfInfo.IsValid())
		{
			OutInfos.RemoveSingle(PlayerSelfInfo);
		}
	}
}

void UGLCGameInstance::OnPlayerTalkingStateChanged(const FUniqueNetIdRepl& InNetID, 
	bool bIsTbMicrophoneOpenalking)
{
	if (UGLCOnlineManageComponent* InCom = UGLCPlatformManageLibrary::GetGLCOnlineManagerComponent())
	{
		if (UGLCOnlineVoiceObject* InObject = InCom->GLCOnlineVoiceChatObject.GetDefaultObject())
		{
			if (FGLCPlayerInfo* FInfo = PlayerVoiceInfos.FindByPredicate([&](const FGLCPlayerInfo& InMyInfo) {return InMyInfo.NetID == InNetID; }))
			{
				FInfo->bMicrophoneOpen = bIsTbMicrophoneOpenalking;
				FInfo->bIsMute = UGLCVoiceLibrary::IsPlayerMuted(InNetID);
				InObject->OnPlayerTalkingStateChange(*FInfo);
			}
			else
			{
				if (APlayerController* InPlayerController = UGLCPlatformManageLibrary::GetPlayerController())
				{
					if (InPlayerController->PlayerState)
					{
						FGLCPlayerInfo NewInfo;
						NewInfo.NetID = InNetID;
						NewInfo.PlayerName = InPlayerController->PlayerState->GetPlayerName();
						NewInfo.bIsMute = UGLCVoiceLibrary::IsPlayerMuted(InNetID);
						NewInfo.bMicrophoneOpen = bIsTbMicrophoneOpenalking;

						PlayerVoiceInfos.Add(NewInfo);
						InObject->OnPlayerTalkingStateChange(NewInfo);
					}
				}
			}
		}
	}
}

void UGLCGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UGLCGameInstance::OnStart()
{
	Super::OnStart();
}

bool UGLCGameInstance::IsInSession()
{
	return bIsInSession;
}

void UGLCGameInstance::SetIsInSession(bool b)
{
	bIsInSession = b;
}
