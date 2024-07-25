#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "PlatformManage/OnlineSessions/Common/GLCOnlineSessionsType.h"
#include "PlatformManage/OnlineSessions/UI/Core/UI_MenuBase.h"
#include "UI_SingleRusult.generated.h"

struct FGLCOnlineSessionSearchResult;
class UImage;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UUI_SingleRusult : public UUI_MenuBase
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	UImage* MapImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoomNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameModeText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNumText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PingText;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	
	FGLCOnlineSessionSearchResult SessionResult;
public:
	virtual void NativeConstruct() override;
	bool InitResult(const FGLCOnlineSessionSearchResult& InResult);

protected:
	UFUNCTION()
	void JoinButtonPress();
	
};
