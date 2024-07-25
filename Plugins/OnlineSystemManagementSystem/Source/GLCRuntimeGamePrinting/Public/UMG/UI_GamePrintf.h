#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GLCRuntimeGamePrintingType.h"
#include "UI_GamePrintf.generated.h"

class UWidgetAnimation;
class URichTextBlock;
/**
 *
 */
UCLASS()
class GLCRUNTIMEGAMEPRINTING_API UUI_GamePrintf : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* TextBlock;
public:
	UUI_GamePrintf(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct();

	virtual void NativeDestruct();

	void WidgetConstruct();

	UFUNCTION()
	void PlayTextAnim();

	void SetText(const GLCRuntimeGamePrinting::FPrintSlot &InMySlot);
	void SetText(const FText& InText);
	void SetText(const FText& InText,const FString &InColor);

	UFUNCTION()
	void SetImage(const FString& InImgID);

	UFUNCTION()
	void Die();

	UFUNCTION()
	void SetDieDelayTime(float InTime) { DieDelayTime = InTime; }
protected:
	UWidgetAnimation* GetNameWidgetAnimation(const FString& InWidgetAnimName);
public:
	void PlayWidgetAnim(const FString& InWidgetAnimName);

protected:
	float DieDelayTime;
	FTimerHandle DieDelayTimeHandle;
};
