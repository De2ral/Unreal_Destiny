// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UHUDWidget::UpdateSkillCoolTime(float cur, float max)
{
    if (SkillCoolTimeHUD)
        SkillCoolTimeHUD->SetPercent(cur / max);
}

void UHUDWidget::UpdateGrenadeCoolTime(float cur, float max)
{
    if (GrenadeCoolTimeHUD)
    {
        GrenadeCoolTimeHUD->SetPercent(cur / max);
    }
}


void UHUDWidget::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
    if (AmmoText)
    {
        AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo)));
    }
}