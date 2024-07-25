#pragma once

#include "CoreMinimal.h"
#include "PlatformManage/Common/OnlineSubsystemManage.h"
#include "Interfaces/VoiceInterface.h"

#include "OnlineVoiceManage.generated.h"

struct FUniqueNetIdRepl;
class APlayerController;

/**
 * 
 */
UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UOnlineVoiceManage : public UOnlineSubsystemManage
{
	GENERATED_BODY()
	
public:
	UOnlineVoiceManage();
	static UOnlineVoiceManage* Get();

	void ToggleSpeaking(bool bStart);

	void GameplayMutePlayer(const FUniqueNetIdRepl& PlayerNetId);
	void GameplayUnmutePlayer(const FUniqueNetIdRepl& PlayerNetId);

	bool IsPlayerMuted(const FUniqueNetIdRepl& PlayerId);

	void OnPlayerTalkingStateChange(APlayerController* InPlayerController,bool bIsTalking);

protected:
	IOnlineVoicePtr OnlineVoice;

};
