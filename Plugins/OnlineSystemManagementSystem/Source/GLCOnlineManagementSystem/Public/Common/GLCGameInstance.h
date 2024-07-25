// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GLCGameInstance.generated.h"

class UUI_LoadingMap;
struct FGLCPlayerInfo;
struct FUniqueNetIdRepl;

/**
 * 
 */
UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UGLCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	TArray<FGLCPlayerInfo> PlayerVoiceInfos;

public:
	UGLCGameInstance();
	
	TSubclassOf<UUI_LoadingMap> LoadingMapClass;

public:
	void GetAllPlayerInfo(TArray<FGLCPlayerInfo>& OutInfos, bool bGetSelfInfo = true);
	void OnPlayerTalkingStateChanged(const FUniqueNetIdRepl& InNetID, bool bIsTbMicrophoneOpenalking);

	bool IsInSession();
	void SetIsInSession(bool b);

protected:
	virtual void Shutdown()override;
	virtual void OnStart()override;

	//是否在会话中，加入地图后为true
	bool bIsInSession;
};
