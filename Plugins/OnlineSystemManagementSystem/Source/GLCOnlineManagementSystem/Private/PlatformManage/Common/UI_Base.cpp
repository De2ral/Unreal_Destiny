

#include "PlatformManage/Common/UI_Base.h"

#include "Animation/WidgetAnimation.h"
#include "GameFramework/HUD.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"


UWidgetAnimation* UUI_Base::GetNameWidgetAnimation(const FString& InWidgetName)
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

void UUI_Base::PlayWidgetAnimForward(const FString& InWidgetName)
{
	if (UWidgetAnimation* MyTempAnim = GetNameWidgetAnimation(InWidgetName))
	{
		PlayAnimationForward(MyTempAnim);
	}
}

void UUI_Base::PlayWidgetAnimReverse(const FString& InWidgetName)
{
	if (UWidgetAnimation* MyTempAnim = GetNameWidgetAnimation(InWidgetName))
	{
		PlayAnimationReverse(MyTempAnim);
	}
}

AHUD* UUI_Base::GetHUD()
{
	if (APlayerController* InPlayerController = GetWorld()->GetFirstPlayerController())
	{
		return InPlayerController->GetHUD();
	}
	return nullptr;
}

UWidget* UUI_Base::K2_GetFather()
{
	checkf(Father, TEXT("Father Widget was forgot to set !"));
	return Father;
}
