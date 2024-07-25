#pragma once

#include "PlatformManage/OnlineSessions/UI/Core/UI_MenuBase.h"
#include "CoreMinimal.h"
#include "PlatformManage/OnlineSessions/Common/GLCOnlineSessionsType.h"
#include "UI_Password.generated.h"

/**
 * 
 */
UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UUI_Password : public UUI_MenuBase
{
	GENERATED_BODY()

public:
	void Init(const FGLCOnlineSessionSearchResult& InResult,const FPlatformManageOptionalParam&
		InParam);
	UFUNCTION(BlueprintCallable,Category="UI_Password")
	bool EnterPassword(FString InPassword);
	
protected:
	FGLCOnlineSessionSearchResult SessionResult;
	FPlatformManageOptionalParam OptionParam;
	FString Password;
	
};
