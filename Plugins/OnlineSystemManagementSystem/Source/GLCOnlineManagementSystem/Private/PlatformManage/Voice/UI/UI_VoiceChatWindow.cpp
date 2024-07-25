// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformManage/Voice/UI/UI_VoiceChatWindow.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "PlatformManage/Voice/UI/Elements/UI_VoiceChatInfo.h"
#include <PlatformManage/Voice/UI/Core/UI_VoiceChatMain.h>
#include <PlatformManage/Common/GLCPlatformManageLibrary.h>
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformManage/Voice/GLCVoiceLibrary.h"



void UUI_VoiceChatWindow::UpdateVoiceChatWindow()
{
	PlayerList->ClearChildren();

	checkf(UGLCVoiceLibrary::VoiceChatInfoClass,TEXT("VoiceChatInfoClass is not valid ?"));

	TArray<FGLCPlayerInfo> PlayerInfos;
	UGLCPlatformManageLibrary::GetAllPlayerInfo(PlayerInfos,false);

	for (FGLCPlayerInfo& Temp : PlayerInfos)
	{
		if (UUI_VoiceChatInfo* InNewInfo = CreateWidget<UUI_VoiceChatInfo>(GetWorld(),UGLCVoiceLibrary::VoiceChatInfoClass))
		{
			InNewInfo->SetFather(K2_GetFather());
			InNewInfo->InitVoiceChatInfo(Temp);
			if (UScrollBoxSlot* InSlot = Cast<UScrollBoxSlot>(PlayerList->AddChild(InNewInfo)))
			{
				InSlot->SetPadding(30.f);
			}
		}
	}
}
