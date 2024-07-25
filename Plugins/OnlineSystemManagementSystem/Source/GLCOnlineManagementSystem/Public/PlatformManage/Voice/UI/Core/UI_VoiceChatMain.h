// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlatformManage/Common/UI_Base.h"

#include "UI_VoiceChatMain.generated.h"

class UScrollBox;
class UUI_VoiceChatWindow;

/**
 * 
 */
UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UUI_VoiceChatMain : public UUI_Base
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	UUI_VoiceChatWindow* UI_VoiceChatWindow;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayVoiceChatInAnim();
	void UpdatePlayerList();

public:
	void SetupUI();

};
