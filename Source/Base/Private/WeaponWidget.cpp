// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


void UWeaponWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ScopeCrossImage->SetVisibility(ESlateVisibility::Hidden);
    BaseCrossImage->SetVisibility(ESlateVisibility::Visible);
}

void UWeaponWidget::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
    if (AmmoText)
    {
        AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo)));
    }
}

void UWeaponWidget::SetScopeSize(float NewSize)
{
    if (ScopeCrossImage)
    {
       ScopeCrossImage->SetRenderScale(FVector2D(NewSize, NewSize));
    }
}


void UWeaponWidget::ImageMove(float NewX, USkeletalMeshComponent* Object)
{
    UE_LOG(LogTemp, Warning, TEXT("ImageMove : %f"),NewX);
    FVector2D TempPosition = FVector2D(NewX,0.5f);
    if (ScopeCrossImage && Object)
    {
        ScopeCrossImage->SetRenderTranslation(TempPosition);
        if(TempPosition.X > 400.0f)
        {
            ScopeCrossImage->SetVisibility(ESlateVisibility::Hidden);
            BaseCrossImage->SetVisibility(ESlateVisibility::Visible);
            Object->SetVisibility(true);
        }
        else
        {
            ScopeCrossImage->SetVisibility(ESlateVisibility::Visible);
            BaseCrossImage->SetVisibility(ESlateVisibility::Hidden);
            Object->SetVisibility(false);
        }
    }

}