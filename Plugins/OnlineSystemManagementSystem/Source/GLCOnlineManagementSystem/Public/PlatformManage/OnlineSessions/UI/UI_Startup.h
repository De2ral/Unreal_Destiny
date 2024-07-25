#pragma once

#include "CoreMinimal.h"
#include "PlatformManage/OnlineSessions/UI/Core/UI_MenuBase.h"
#include "UI_Startup.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UUI_Startup : public UUI_MenuBase
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UButton* CreateButton;
	UPROPERTY(meta = (BindWidget))
	UButton* SearchButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ConfigSetButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

protected:
	virtual void NativeConstruct()override;

	UFUNCTION()
	void CreateButtonPress();
	UFUNCTION()
	void SearchButtonPress();
	UFUNCTION()
	void ConfigSetButtonPress();
	UFUNCTION()
	void QuitButtonPress();

};
