#include "UMG/UI_GamePrintf.h"

#include "Animation/WidgetAnimation.h"
#include "Components/RichTextBlock.h"
#include "TimerManager.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"

UUI_GamePrintf::UUI_GamePrintf(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	, DieDelayTime(7.f)
{

}

void UUI_GamePrintf::WidgetConstruct()
{
	GetWorld()->GetTimerManager().SetTimer(DieDelayTimeHandle, this, &UUI_GamePrintf::Die, DieDelayTime);
	
	PlayTextAnim();
}

void UUI_GamePrintf::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_GamePrintf::Die()
{
	if (GetWorld()->GetTimerManager().TimerExists(DieDelayTimeHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(DieDelayTimeHandle);
	}
	
	RemoveFromParent();
}

void UUI_GamePrintf::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_GamePrintf::PlayTextAnim()
{
	PlayWidgetAnim(TEXT("PlayText"));
}

void UUI_GamePrintf::SetText(const FText& InText,const FString& InColor)
{
	TextBlock->SetText(FText::FromString(
		GLCRuntimeGamePrinting::FPrintSlot()
		.AddSlot()
		.Color(InColor)
		[
			InText.ToString()
		].ToString()));
}

void UUI_GamePrintf::SetText(const FText & InText)
{
	TextBlock->SetText(InText);
}

void UUI_GamePrintf::SetImage(const FString& InImgID)
{
	TextBlock->SetText(FText::FromString(
		GLCRuntimeGamePrinting::FPrintSlot()
		.AddSlot()
		.Image(InImgID)
		.ToString()));
}

void UUI_GamePrintf::SetText(const GLCRuntimeGamePrinting::FPrintSlot& InMySlot)
{
	TextBlock->SetText(FText::FromString(InMySlot.ToString()));
}

UWidgetAnimation * UUI_GamePrintf::GetNameWidgetAnimation(const FString& InWidgetAnimName)
{
	if (UWidgetBlueprintGeneratedClass* WidgetBlueprintGenerated = Cast<UWidgetBlueprintGeneratedClass>(GetClass()))
	{
		TArray<UWidgetAnimation*> TArrayAnimations = WidgetBlueprintGenerated->Animations;

		UWidgetAnimation** MyTempAnimation = TArrayAnimations.FindByPredicate(
			[&](const UWidgetAnimation* OurAnimation)
			{
				return OurAnimation->GetFName().ToString() == (InWidgetAnimName + FString("_INST"));
			});

		return *MyTempAnimation;
	}

	return nullptr;
}

void UUI_GamePrintf::PlayWidgetAnim(const FString& InWidgetAnimName)
{
	if (UWidgetAnimation* MyTempAnimation = GetNameWidgetAnimation(InWidgetAnimName))
	{
		PlayAnimation(MyTempAnimation);
	}
}
