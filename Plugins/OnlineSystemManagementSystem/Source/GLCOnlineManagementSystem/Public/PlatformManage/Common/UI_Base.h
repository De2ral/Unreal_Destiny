#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "UI_Base.generated.h"

class AHUD;



/**
 * 
 */
UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UUI_Base : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UWidgetAnimation* GetNameWidgetAnimation(const FString& InWidgetName);
	void PlayWidgetAnimForward(const FString& InWidgetName);
	void PlayWidgetAnimReverse(const FString& InWidgetName);

	template<class T>
	T* GetFather()
	{
		checkf(Father,TEXT("Father Widget was forgot to set !"));
		return Cast<T>(Father);
	}
	AHUD* GetHUD();

	UFUNCTION(BlueprintCallable,Category = "UI_Base")
	void SetFather(UWidget* InNew){Father = InNew;}
	UFUNCTION(BlueprintCallable, Category = "UI_Base")
	UWidget* K2_GetFather();

private:
	UPROPERTY()
	UWidget* Father;
};
