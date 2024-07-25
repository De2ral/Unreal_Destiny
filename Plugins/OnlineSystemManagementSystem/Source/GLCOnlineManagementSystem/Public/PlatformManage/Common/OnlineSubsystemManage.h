#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlatformManage/Common/PlatformManageType.h"
#include "Common/Type/GLCCommonType.h"

#include "OnlineSubsystemManage.generated.h"

class UGLCOnlineManageComponent;
struct FPlatformManageOptionalParam;
struct FOnlineSubsystemDataManage;


UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UOnlineSubsystemManage : public UGameInstanceSubsystem
{
	GENERATED_UCLASS_BODY()

public:
	UGLCOnlineManageComponent* GetOnlineManageComponent();

	void RemoveRequest(FString InRemoveKey,TFunction<void(const FPlatformManageOptionalParam& OutParam)> Fun);
	const FPlatformManageOptionalParam& GetRequest(FString& InKey);
	bool PublishRequest(const FPlatformManageOptionalParam& InParam,const FString& InRequestKey);
private:
	TMap<FName,FPlatformManageOptionalParam> CurrentRequest;
};
