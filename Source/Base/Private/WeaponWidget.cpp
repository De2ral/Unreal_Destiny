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


void UWeaponWidget::ImageMove(float NewX, UStaticMeshComponent* Object)
{
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

void UWeaponWidget::SetTextureBasedOnGunType(int GunType, bool isAiming)
{
    FString TexturePath;

    switch (GunType)
    {
    case 0:
        if(isAiming)
            TexturePath = TEXT("/Script/Engine.Texture2D'/Game/Weapon/assets/Crosshair/Pistol_Aim.Pistol_Aim'");
        else
            TexturePath = TEXT("/Script/Engine.Texture2D'/Game/Weapon/assets/Crosshair/Pitol_Idle.Pitol_Idle'");
        break;
    case 1:
        if(isAiming)
            TexturePath = TEXT("/Script/Engine.Texture2D'/Game/Weapon/assets/Crosshair/Rifle_Aim.Rifle_Aim'");
        else
            TexturePath = TEXT("/Script/Engine.Texture2D'/Game/Weapon/assets/Crosshair/Rifle_Idle.Rifle_Idle'");
        break;
    case 2:
        if(isAiming)
            TexturePath = TEXT("/Script/Engine.Texture2D'/Game/Weapon/assets/Crosshair/Shotgun_Anim.Shotgun_Anim'");
        else
            TexturePath = TEXT("/Script/Engine.Texture2D'/Game/Weapon/assets/Crosshair/Shotgun_Idle.Shotgun_Idle'");
        break;
    case 3:
        if(!isAiming)
            TexturePath = TEXT("/Script/Engine.Texture2D'/Game/Weapon/assets/Crosshair/Sniper_Idle.Sniper_Idle'");
        break;
    case 4:
        if(!isAiming)
            TexturePath = TEXT("/Script/Engine.Texture2D'/Game/Weapon/assets/Crosshair/Launcher_Idle.Launcher_Idle'");
        break;

    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown gun type!"));
        return;
    }

    UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, *TexturePath);
    if (LoadedTexture)
    {
        UE_LOG(LogTemp, Warning, TEXT("cross success"));
        BaseCrossImage->SetBrushFromTexture(LoadedTexture);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load texture from path: %s"), *TexturePath);
    }
}