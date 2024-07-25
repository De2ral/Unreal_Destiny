

#include "PlatformManage/OnlineSessions/UI/UI_Startup.h"

#include "Components/Button.h"
#include "PlatformManage/OnlineSessions/UI/Core/UI_MenuMain.h"
#include <Kismet/KismetSystemLibrary.h>
#include <PlatformManage/OnlineSessions/Common/GLCOnlineSessionsLibrary.h>
#include "PlatformManage/OnlineSessions/SaveGame/GLCOnlineSessionsConfig.h"



void UUI_Startup::NativeConstruct()
{
	if(CreateButton)
	{
		CreateButton->OnReleased.AddDynamic(this,&UUI_Startup::CreateButtonPress);
	}
	if(SearchButton)
	{
		SearchButton->OnReleased.AddDynamic(this,&UUI_Startup::SearchButtonPress);
	}
	if(ConfigSetButton)
	{
		ConfigSetButton->OnReleased.AddDynamic(this,&UUI_Startup::ConfigSetButtonPress);
	}
	if(QuitButton)
	{
		QuitButton->OnReleased.AddDynamic(this,&UUI_Startup::QuitButtonPress);
	}
}

void UUI_Startup::CreateButtonPress()
{
	if(UUI_MenuMain* InMain = GetFather())
	{
		if (UGLCOnlineSessionsConfig* InConfig = UGLCOnlineSessionsLibrary::GetOnlineSessionsConfigFromSlot())
		{
			InMain->ShowLoading();
			InMain->CreateSession(InConfig->OnlineSessionsConfigSpec.SessionCreateSettings);
		}
	}
}

void UUI_Startup::SearchButtonPress()
{
	if(UUI_MenuMain* InMain = GetFather())
	{
		InMain->PlaySearchInAnim();
		InMain->ShowLoading();
		InMain->FindSession();
	}
}

void UUI_Startup::ConfigSetButtonPress()
{
	if (UUI_MenuMain* InMain = GetFather())
	{
		InMain->PlayConfigInAnim();
	}
}

void UUI_Startup::QuitButtonPress()
{
	if (GetWorld())
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),EQuitPreference::Quit,
			true);
	}
}
