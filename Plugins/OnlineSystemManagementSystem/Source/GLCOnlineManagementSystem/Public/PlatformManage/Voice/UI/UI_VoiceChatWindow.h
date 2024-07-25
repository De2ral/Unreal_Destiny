// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlatformManage/Common/UI_Base.h"
#include "UI_VoiceChatWindow.generated.h"


class UScrollBox;
class UUI_VoiceChatInfo;

/**
 * 
 */
UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UUI_VoiceChatWindow : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UScrollBox* PlayerList;

protected:
	UPROPERTY(EditAnywhere,Category = "VoiceUI")
	TSubclassOf<UUI_VoiceChatInfo> VoiceChatInfoClass;

public:
	UFUNCTION(BlueprintCallable,Category = "UI_VoiceChatWindow")
	void UpdateVoiceChatWindow();

};
