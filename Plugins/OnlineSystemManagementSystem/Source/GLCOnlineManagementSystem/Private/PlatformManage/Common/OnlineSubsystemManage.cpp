#include "PlatformManage/Common/OnlineSubsystemManage.h"

#include "Common/Components/GLCOnlineManageComponent.h"
#include "Common/Components/GLCOnlineManageComponent.h"
#include "Common/Type/GLCCommonType.h"
#include <PlatformManage/Common/GLCPlatformManageLibrary.h>


FOnlineSubsystemDataManage* FOnlineSubsystemDataManage::OnlineSubsystemDataManage = nullptr;

UOnlineSubsystemManage::UOnlineSubsystemManage(const FObjectInitializer& Initializer)
{
	
}

UGLCOnlineManageComponent* UOnlineSubsystemManage::GetOnlineManageComponent()
{
	return UGLCPlatformManageLibrary::GetGLCOnlineManagerComponent();
}

void UOnlineSubsystemManage::RemoveRequest(FString InRemoveKey, 
	TFunction<void(const FPlatformManageOptionalParam& OutParam)> Fun)
{
	if (FPlatformManageOptionalParam* InParam = CurrentRequest.Find(*InRemoveKey))
	{
		Fun(*InParam);
		CurrentRequest.Remove(*InRemoveKey);
	}
}

const FPlatformManageOptionalParam& UOnlineSubsystemManage::GetRequest(FString& InKey)
{
	if (FPlatformManageOptionalParam* InParam = CurrentRequest.Find(*InKey))
	{
		return *InParam;
	}
	return FPlatformManageOptionalParam::DefauOptionalParam;
}

bool UOnlineSubsystemManage::PublishRequest(const FPlatformManageOptionalParam& InParam,
	const FString& InRequestKey)
{
	if (CurrentRequest.Find(*InRequestKey))
	{
		/*UE_LOG(LogGLCOnlineManagementSystem, Warning, TEXT("The Request now is process ! Do not request again"));
		UE_LOG(LogGLCOnlineManagementSystem, Warning, TEXT("The Request now is process ! Do not request again"));*/
		return false;
	}
	else
	{
		CurrentRequest.Add(*InRequestKey,InParam);
		return true;
	}
	return false;
}

