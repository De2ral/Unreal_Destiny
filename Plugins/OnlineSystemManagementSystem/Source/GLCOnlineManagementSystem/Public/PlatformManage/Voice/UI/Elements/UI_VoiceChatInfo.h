// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlatformManage/Common/UI_Base.h"
#include "Common/Type/GLCCommonType.h"

#include "UI_VoiceChatInfo.generated.h"

class UTextBlock;
class UCheckBox;

/**
 * 
 */
UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UUI_VoiceChatInfo : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ChatStateText;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* MuteCheckBox;

protected:
	FGLCPlayerInfo PlayerInfo;

protected:
	virtual void NativeConstruct()override;
	UFUNCTION()
	void OnMyCheckBoxStateChanged(bool bIsChecked);

public:
	void InitVoiceChatInfo(const FGLCPlayerInfo& InPlayerInfo);

};
