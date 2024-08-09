// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponWidget.generated.h"


/**
 * 
 */
UCLASS()
class BASE_API UWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* AmmoText;
};
