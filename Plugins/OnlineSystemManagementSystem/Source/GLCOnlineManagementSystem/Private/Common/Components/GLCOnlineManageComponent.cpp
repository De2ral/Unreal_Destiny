#include "Common/Components/GLCOnlineManageComponent.h"
#include "GameFramework/GameModeBase.h"
#include <PlatformManage/Common/GLCPlatformManageLibrary.h>
#include <PlatformManage/OnlineSessions/Common/GLCOnlineSessionsLibrary.h>
#include "PlatformManage/Common/PlatformManageType.h"
#include "Engine/World.h"
#include <PlatformManage/Voice/GLCVoiceLibrary.h>
#include "GameFramework/PlayerState.h"
#include "Common/GLCGameInstance.h"
#include "PlatformManage/Voice/UI/Core/UI_VoiceChatMain.h"
#include "PlatformManage/OnlineSessions/UI/Core/UI_MenuMain.h"
#include "PlatformManage/Voice/Core/GLCOnlineVoiceObject.h"



UGLCOnlineManageComponent::UGLCOnlineManageComponent()
{
	PrimaryComponentTick.bCanEverTick = 1;

	GLCOnlineSessionObjectClasses.Add(UGLCOnlineSessionObject::StaticClass());
	GLCUserCloundObjectClasses.Add(UGLCUserCloundObject::StaticClass());
	GLCOnlineVoiceChatObject = UGLCOnlineVoiceObject::StaticClass();
	
}

void UGLCOnlineManageComponent::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* InPlayerController = GetOwner<APlayerController>())
	{
		UGLCPlatformManageLibrary::RegisterGLCOnlineManagerComponentFromController(InPlayerController);
	}
}

void UGLCOnlineManageComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
}

// Called every frame
void UGLCOnlineManageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UUI_VoiceChatMain* UGLCOnlineManageComponent::GetVoiceChatMain()
{
	return VoiceChatMain;
}

void UGLCOnlineManageComponent::SetVoiceChatMain(UUI_VoiceChatMain* val)
{
	VoiceChatMain = val;
}

void UGLCOnlineManageComponent::Server_StartGame_Implementation()
{
	FString URL;
	if (UGLCOnlineSessionsLibrary::GetPathToGameMap(URL) && GetWorld())
	{
		if (AGameModeBase* InGameMode = GetWorld()->GetAuthGameMode())
		{
			InGameMode->bUseSeamlessTravel = true;
			GetWorld()->ServerTravel(URL);
		}
	}
	
}

void UGLCOnlineManageComponent::Server_StartGameByKey_Implementation(const int32 InKey)
{
	if (FMapSettingSpec* InMapSetting = UGLCOnlineSessionsLibrary::GetSettingSpecByKey(InKey))
	{
		FString Path = InMapSetting->GetMapPath();
		Path += TEXT("?listen");
		if (AGameModeBase* InGameMode = GetWorld()->GetAuthGameMode())
		{
			InGameMode->bUseSeamlessTravel = true;
			GetWorld()->ServerTravel(Path);
		}
	}
}

void UGLCOnlineManageComponent::Server_OnPlayerTalkingStateChange_Implementation(const FUniqueNetIdRepl& InNetID,
	bool bMicrophoneOpen)
{
	if (UWorld* InWorld = GetWorld())
	{
		for (auto CI = InWorld->GetPlayerControllerIterator();CI;++CI)
		{
			if (CI && CI->Get())
			{
				if (UGLCOnlineManageComponent* InCom = UGLCPlatformManageLibrary::GetGLCOnlineManageComponentFormObject(CI->Get()))
				{
					InCom->Client_OnPlayerTalkingStateChange(InNetID, bMicrophoneOpen);
				}
			}
		}
	}
}

void UGLCOnlineManageComponent::Client_OnPlayerTalkingStateChange_Implementation(
	const FUniqueNetIdRepl& InNetID, bool bMicrophoneOpen)
{
	checkf(GLCOnlineVoiceChatObject, TEXT("GLCOnlineVoiceObject is not valid !"));
	if (UGLCGameInstance* InGI = UGLCPlatformManageLibrary::GetGLCGameInstance())
	{
		InGI->OnPlayerTalkingStateChanged(InNetID,bMicrophoneOpen);
	}
}

void UGLCOnlineSessionObject::OnCreateSessionComplete_Implementation(bool bWasSuccessful, const FPlatformManageOptionalParam& OptionalParam)
{

}

void UGLCOnlineSessionObject::OnFindSessionComplete_Implementation(const TArray<FGLCOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful, const FPlatformManageOptionalParam& OptionalParam)
{

}

void UGLCOnlineSessionObject::OnJoinSessionComplete_Implementation(EjoinSessionCompleteType JoinType,
	const FPlatformManageOptionalParam& OptionalParam)
{

}

void UGLCOnlineSessionObject::OnLoginComplete_Implementation(int32 LocalUserNum, bool bWasSuccessful,
	const FUniqueNetIdRepl& UserId, const FString& Error, const FPlatformManageOptionalParam& OptionalParam)
{
	
}

void UGLCOnlineSessionObject::OnDestroySessionComplete_Implementation(bool bWasSuccessful,
	const FPlatformManageOptionalParam& OptionalParam)
{
}

void UGLCUserCloundObject::OnReadUserFileComplete_Implementation(bool bSuccessful, const FUniqueNetIdRepl& NetID, const FString& FileName, const FPlatformManageOptionalParam& OptionalParam)
{

}

void UGLCUserCloundObject::OnWriteUserFileInProgress_Implementation(int32 CurrentWriteBytes, const FUniqueNetIdRepl& NetID, const FString& FileName, const FPlatformManageOptionalParam& OptionalParam)
{

}

void UGLCUserCloundObject::OnWriteUserFileComplete_Implementation(bool bSuccessful, const FUniqueNetIdRepl& InNetID, const FString& InFileName, const FPlatformManageOptionalParam& OptionalParam)
{

}

void UGLCUserCloundObject::OnDeleteUserFileComplete_Implementation(bool bSuccessful, const FUniqueNetIdRepl& InNetID, const FString& InFileName, const FPlatformManageOptionalParam& OptionalParam)
{

}

void UGLCUserCloundObject::OnEnumerateUserFileComplete_Implementation(bool bSuccessful, const FUniqueNetIdRepl& InNetID, const FPlatformManageOptionalParam& OptionalParam)
{

}

void UGLCOnlineSessionObject::OnLogStatusChanged_Implementation(int32 LocalUserNum, EGLCLoginState OldStatus, EGLCLoginState NewStatus, const FUniqueNetIdRepl& NewId)
{

}
