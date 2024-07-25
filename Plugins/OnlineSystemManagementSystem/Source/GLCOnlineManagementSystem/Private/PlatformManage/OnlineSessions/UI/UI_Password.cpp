#include "PlatformManage/OnlineSessions/UI/UI_Password.h"

#include "PlatformManage/OnlineSessions/Common/GLCOnlineSessionsLibrary.h"

void UUI_Password::Init(const FGLCOnlineSessionSearchResult& InResult,const FPlatformManageOptionalParam&
	InParam)
{
	Password = InResult.RoomPassword;
	OptionParam = InParam;
	SessionResult = InResult;
}

bool UUI_Password::EnterPassword(FString InPassword)
{
	if(InPassword == Password)
	{
		if(const FOnlineSessionSearchResult* R = UGLCOnlineSessionsLibrary::ConvertSearchResult(SessionResult))
		{
			UGLCOnlineSessionsLibrary::JoinSession(*R,OptionParam);
			return true;
		}
	}
	return false;
}
