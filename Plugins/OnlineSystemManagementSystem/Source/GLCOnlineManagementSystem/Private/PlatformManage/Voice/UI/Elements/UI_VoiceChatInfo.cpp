// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformManage/Voice/UI/Elements/UI_VoiceChatInfo.h"

#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "PlatformManage/Voice/GLCVoiceLibrary.h"
#include "PlatformManage/Common/GLCPlatformManageLibrary.h"

void UUI_VoiceChatInfo::NativeConstruct()
{
	Super::NativeConstruct();

	MuteCheckBox->OnCheckStateChanged.AddDynamic(this,&UUI_VoiceChatInfo::OnMyCheckBoxStateChanged);
}

void UUI_VoiceChatInfo::OnMyCheckBoxStateChanged(bool bIsChecked)
{
	if (PlayerInfo.IsValid())
	{
		UGLCVoiceLibrary::SetIsMutePlayer(PlayerInfo.NetID,bIsChecked);
	}
}

void UUI_VoiceChatInfo::InitVoiceChatInfo(const FGLCPlayerInfo& InPlayerInfo)
{
	if (PlayNameText && ChatStateText && MuteCheckBox)
	{
		FString NewPlayerName;
		if (InPlayerInfo.PlayerName.Len() > 4)
		{
			auto Chars = InPlayerInfo.PlayerName.GetCharArray();
			NewPlayerName.Append(Chars.GetData(),3);
			NewPlayerName += TEXT("...");
		}
		else
		{
			NewPlayerName = InPlayerInfo.PlayerName;
		}
		PlayNameText->SetText(FText::FromString(NewPlayerName));
		ChatStateText->SetText(FText::FromString(InPlayerInfo.bMicrophoneOpen ? TEXT("Yes") : TEXT("No")));
		MuteCheckBox->SetIsChecked(InPlayerInfo.bIsMute);

		if (InPlayerInfo == UGLCPlatformManageLibrary::GetSelfInfo())
		{
			SetIsEnabled(false);
		}
		PlayerInfo = InPlayerInfo;
	}
}
