

#include "PlatformManage/OnlineSessions/UI/Core/UI_MenuBase.h"

#include "Animation/WidgetAnimation.h"
#include "GameFramework/HUD.h"
#include "PlatformManage/OnlineSessions/UI/Core/UI_MenuMain.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"


UWidgetAnimation* UUI_MenuBase::GetNameWidgetAnimation(const FString& InWidgetName)
{
	if (UWidgetBlueprintGeneratedClass* WidgetBlueprintGenerated = Cast<UWidgetBlueprintGeneratedClass>(GetClass()))
	{
		TArray<UWidgetAnimation*> TArrayAnimations = WidgetBlueprintGenerated->Animations;
		UWidgetAnimation** MyTempAnimation = TArrayAnimations.FindByPredicate(
			[&](const UWidgetAnimation* OurAnimation)
			{
				return OurAnimation->GetFName().ToString() == (InWidgetName + FString("_INST"));
			});
		return MyTempAnimation ? *MyTempAnimation : nullptr;
	}

	return nullptr;
}

void UUI_MenuBase::PlayWidgetAnimForward(const FString& InWidgetName)
{
	if (UWidgetAnimation* MyTempAnim = GetNameWidgetAnimation(InWidgetName))
	{
		PlayAnimationForward(MyTempAnim);
	}
}

void UUI_MenuBase::PlayWidgetAnimReverse(const FString& InWidgetName)
{
	if (UWidgetAnimation* MyTempAnim = GetNameWidgetAnimation(InWidgetName))
	{
		PlayAnimationReverse(MyTempAnim);
	}
}

AHUD* UUI_MenuBase::GetHUD()
{
	if (APlayerController* InPlayerController = GetWorld()->GetFirstPlayerController())
	{
		return InPlayerController->GetHUD();
	}
	return nullptr;
}

UUI_MenuMain* UUI_MenuBase::K2_GetFather()
{
	return Cast<UUI_MenuMain>(Father);
}

