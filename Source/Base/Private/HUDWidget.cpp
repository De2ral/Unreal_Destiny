// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UHUDWidget::UpdateSkillCoolTime(float cur, float max)
{
    if (SkillCoolTimeHUD)
        SkillCoolTimeHUD->SetPercent(cur / max);
    //GEngine->AddOnScreenDebugMessage(-1,0.5f,FColor::Red, FString::Printf(TEXT("현재 쿨타임 퍼센트 : %f"), cur / max));
}

void UHUDWidget::UpdateGrenadeCoolTime(float cur, float max)
{
    if (GrenadeCoolTimeHUD)
    {
        GrenadeCoolTimeHUD->SetPercent(cur / max);
    }
}

void UHUDWidget::UpdateMeleeCoolTime(float cur, float max)
{
    if (MeleeCoolTimeHUD)
    {
        MeleeCoolTimeHUD->SetPercent(cur / max);
    }
}

void UHUDWidget::UpdateUltimateCoolTime(float cur, float max)
{
    if (UltimateCoolTimeHUD)
    {
        UltimateCoolTimeHUD->SetPercent(cur / max);
    }
}

void UHUDWidget::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
    if (AmmoText)
    {
        AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo)));
    }
}