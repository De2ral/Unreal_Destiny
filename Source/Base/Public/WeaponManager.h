// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponManager.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FWeaponBaseTable : public FTableRowBase
{
	GENERATED_BODY()

	protected:
		UPROPERTY(EditAnywhere,BlueprintReadWrite)
		float Dmg;
		UPROPERTY(EditAnywhere,BlueprintReadWrite)
		int Magsize;
		UPROPERTY(EditAnywhere,BlueprintReadWrite)
		int MaxAmmo;
		UPROPERTY(EditAnywhere,BlueprintReadWrite)
		int currentAmmo;
		UPROPERTY(EditAnywhere,BlueprintReadWrite)
		float range;
		UPROPERTY(EditAnywhere,BlueprintReadWrite)
		int rarity;
		UPROPERTY(EditAnywhere,BlueprintReadWrite)
		TObjectPtr<UStaticMesh> Model;
		UPROPERTY(EditAnywhere,BlueprintReadWrite)
		TObjectPtr<AActor> WeaponActor;

};



