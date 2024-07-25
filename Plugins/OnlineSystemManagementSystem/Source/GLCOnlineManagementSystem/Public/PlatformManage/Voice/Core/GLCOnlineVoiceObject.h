// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "PlatformManage/Common/GLCPlatformObjectBase.h"
#include "Common/Type/GLCCommonType.h"

#include "GLCOnlineVoiceObject.generated.h"

struct FUniqueNetIdRepl;
struct FGLCPlayerInfo;

/**
 * 
 */
UCLASS(Blueprintable)
class GLCONLINEMANAGEMENTSYSTEM_API UGLCOnlineVoiceObject : public UGLCPlatformObjectBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, Category = "OnlineVoice")
	void OnPlayerTalkingStateChange(const FGLCPlayerInfo& InPlayerInfo);
	virtual void OnPlayerTalkingStateChange_Implementation(const FGLCPlayerInfo& InPlayerInfo);

};
