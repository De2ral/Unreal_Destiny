// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class BASE_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void UpdateCurAmmo(int32 CurAmmo);
    void UpdateCurStoredAmmo(int32 StoredAmmo);
    void UpdateSubAmmo1(int32 StoredAmmo);
    void UpdateSubAmmo2(int32 StoredAmmo);
    void UpdateSkillCoolTime(float cur, float max);
    void UpdateGrenadeCoolTime(float cur, float max);
    void UpdateMeleeCoolTime(float cur, float max);
    void UpdateUltimateCoolTime(float cur, float max);

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CurAmmoText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CurStoredAmmoText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SubAmmoText1;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SubAmmoText2;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* SkillCoolTimeHUD;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* GrenadeCoolTimeHUD;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* MeleeCoolTimeHUD;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* UltimateCoolTimeHUD;
};
