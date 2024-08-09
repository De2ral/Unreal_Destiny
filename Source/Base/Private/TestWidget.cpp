// Fill out your copyright notice in the Description page of Project Settings.


#include "TestWidget.h"
#include "Components/TextBlock.h"

void UTestWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    UpdateAmmo(30, 30);  
}

void UTestWidget::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
    if (AmmoText)
    {
        AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo)));
    }
}

