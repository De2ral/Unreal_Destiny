

#include "PlatformManage/OnlineSessions/UI/Element/UI_SingleRusult.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "PlatformManage/OnlineSessions/UI/Core/UI_MenuMain.h"
#include <PlatformManage/OnlineSessions/Common/GLCOnlineSessionsLibrary.h>

void UUI_SingleRusult::NativeConstruct()
{
	Super::NativeConstruct();

	if(JoinButton)
	{
		JoinButton->OnReleased.AddDynamic(this,&UUI_SingleRusult::JoinButtonPress);
	}
}

bool UUI_SingleRusult::InitResult(const FGLCOnlineSessionSearchResult& InResult)
{
	if(MapImage && RoomNameText && GameModeText && PlayerNumText && PingText)
	{
		int32 Ping = InResult.Ping;
		FString OwingName = InResult.OwningUserName;
		int32 MaxPlayerNum = InResult.MaxPlayer;
		int32 CurrentPlayer = InResult.CurrentPlayer;
		int32 MapIDIndex = InResult.MapID;
		FString SessionName = InResult.RoomName;
		
		if(FMapSettingSpec* InSettingSpe = UGLCOnlineSessionsLibrary::GetSettingSpecByKey(MapIDIndex))
		{
			MapImage->SetBrushFromTexture(InSettingSpe->MapImage);
			RoomNameText->SetText(FText::FromString(SessionName));
			GameModeText->SetText(InSettingSpe->GameMapText);
			PlayerNumText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"),CurrentPlayer,MaxPlayerNum)));
			PingText->SetText(FText::FromString(FString::Printf(TEXT("%dms"),Ping)));
			SessionResult = InResult;

			return true;
		}
	}
	return false;
}

void UUI_SingleRusult::JoinButtonPress()
{
	if(SessionResult.SessionInfo.IsValid())
	{
		if(UUI_MenuMain* InMain = GetFather<>())
		{
			InMain->ShowLoading();
			InMain->JoinSession(SessionResult);
		}
	}
}
