// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponWidget.generated.h"

UCLASS()
class BASE_API UWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    void SetScopeSize(float NewSize);

    void ImageMove(float NewX, UStaticMeshComponent* Object, bool IsRifle);

    void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);

    void SetTextureBasedOnGunType(int GunType, bool isAiming);

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* AmmoText;

    UPROPERTY(meta = (BindWidget))
    class UImage* BaseCrossImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* ScopeCrossImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* ScopeCrossImage2;
};
