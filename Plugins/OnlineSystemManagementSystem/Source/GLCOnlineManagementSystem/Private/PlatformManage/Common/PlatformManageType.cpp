#include "PlatformManage/Common/PlatformManageType.h"

#include "PlatformManage/Common/GLCPlatformCoreMinimal.h"
#include "Common/CommonCoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlatformManage/Common/GLCPlatformManageLibrary.h"


FOnlineSubsystemDataManage::FOnlineSubsystemDataManage()
{
	
}

FOnlineSubsystemDataManage* FOnlineSubsystemDataManage::Get()
{
	if (!OnlineSubsystemDataManage)
	{
		OnlineSubsystemDataManage = new FOnlineSubsystemDataManage();
	}
	return OnlineSubsystemDataManage;
}

void FOnlineSubsystemDataManage::Destroy()
{
	if (OnlineSubsystemDataManage)
	{
		delete OnlineSubsystemDataManage;
	}
	OnlineSubsystemDataManage = nullptr;
}

bool FOnlineSubsystemDataManage::GetUniqueID(FUniqueNetIdRepl& OutNetID)
{
	if (UGLCOnlineManageComponent* InCom = FOnlineSubsystemDataManage::Get()->GetOnlineManageComponent())
	{
		return UGLCPlatformManageLibrary::GetUniqueNetIDFromObject(InCom, OutNetID);
	}
	return false;
}

UGLCOnlineManageComponent* FOnlineSubsystemDataManage::GetOnlineManageComponent()
{
	return OnlineManageComponent.IsValid() ? OnlineManageComponent.Get() : nullptr;
}

void FOnlineSubsystemDataManage::SetOnlineManageComponent(UGLCOnlineManageComponent* val)
{
	OnlineManageComponent = val;
}

APlayerController* FOnlineSubsystemDataManage::GetMyPlayerController()
{
	if (MyPlayerController___.IsValid())
	{
		return MyPlayerController___.Get();
	}
	return nullptr;
}

void FOnlineSubsystemDataManage::SetMyPlayerController(APlayerController* val)
{
	MyPlayerController___ = val;
}
