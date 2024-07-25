#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "UI_MenuBase.generated.h"

class UUI_MenuMain;
class AHUD;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UUI_MenuBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UWidgetAnimation* GetNameWidgetAnimation(const FString& InWidgetName);
	void PlayWidgetAnimForward(const FString& InWidgetName);
	void PlayWidgetAnimReverse(const FString& InWidgetName);

	template<class T = UUI_MenuMain>
	T* GetFather()
	{
		check(Father);
		return Cast<T>(Father);
	}
	AHUD* GetHUD();

	UFUNCTION(BlueprintCallable,Category = "UI_MenuBase")
	void SetFather(UWidget* InNew){Father = InNew;}
	UFUNCTION(BlueprintCallable, Category = "UI_MenuBase")
	UUI_MenuMain* K2_GetFather();
private:
	UPROPERTY()
	UWidget* Father;
};
