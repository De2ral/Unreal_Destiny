// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Net/UnrealNetwork.h"


void UWeaponWidget::NativeConstruct()
{
    Super::NativeConstruct();

    BaseCrossImage->SetVisibility(ESlateVisibility::Visible);
    ScopeCrossImage->SetVisibility(ESlateVisibility::Hidden);
    ScopeCrossImage2->SetVisibility(ESlateVisibility::Hidden);
}


void UWeaponWidget::SetScopeSize(float NewSize)
{
    if (ScopeCrossImage || ScopeCrossImage2)
    {
       ScopeCrossImage->SetRenderScale(FVector2D(NewSize, NewSize));
       ScopeCrossImage2->SetRenderScale(FVector2D(NewSize, NewSize));
    }
}


void UWeaponWidget::ImageMove(float NewX, UStaticMeshComponent* Object, bool IsRifle)
{
    FVector2D TempPosition = FVector2D(NewX,0.5f);
    if ((ScopeCrossImage || ScopeCrossImage2) && Object)
    {
        ScopeCrossImage->SetRenderTranslation(TempPosition);

        if(IsRifle)
        {
            if(TempPosition.X > 400.0f)
            {
                BaseCrossImage->SetVisibility(ESlateVisibility::Visible);
                ScopeCrossImage->SetVisibility(ESlateVisibility::Hidden);
                ScopeCrossImage2->SetVisibility(ESlateVisibility::Hidden);
                Object->SetVisibility(true);
            }
            else
            {
                BaseCrossImage->SetVisibility(ESlateVisibility::Hidden);
                ScopeCrossImage->SetVisibility(ESlateVisibility::Visible);
                ScopeCrossImage2->SetVisibility(ESlateVisibility::Hidden);
                Object->SetVisibility(false);
            }
        }
        else
        {
            if(TempPosition.X > 400.0f)
            {
                BaseCrossImage->SetVisibility(ESlateVisibility::Visible);
                ScopeCrossImage->SetVisibility(ESlateVisibility::Hidden);
                ScopeCrossImage2->SetVisibility(ESlateVisibility::Hidden);
                Object->SetVisibility(true);
            }
            else
            {
                BaseCrossImage->SetVisibility(ESlateVisibility::Hidden);
                ScopeCrossImage->SetVisibility(ESlateVisibility::Hidden);
                ScopeCrossImage2->SetVisibility(ESlateVisibility::Visible);
                Object->SetVisibility(false);
            }
        }

        
    }

}

void UWeaponWidget::SetTextureBasedOnGunType(int GunType, bool isAiming)
{
    return;
    UE_LOG(LogTemp, Warning, TEXT("SetTextureBasedOnGunType"));
    UE_LOG(LogTemp, Warning, TEXT("GunType %d"),GunType);
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

    UE_LOG(LogTemp, Warning, TEXT("GunType %s"),*TexturePath);
    UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, *TexturePath);
    if (LoadedTexture)
    {
        if(BaseCrossImage != nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("SetTextureBasedOnGunType success"));
            BaseCrossImage->SetBrushFromTexture(LoadedTexture);
        }
        else
            return;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load texture from path: %s"), *TexturePath);
    }
}