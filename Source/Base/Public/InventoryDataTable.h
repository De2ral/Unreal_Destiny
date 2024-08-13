// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemComponent.h"
#include "InventoryDataTable.generated.h"

UENUM(BlueprintType)
enum class ERarity : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Rare UMETA(DisplayName = "Rare"),
	Legend UMETA(DisplayName = "Legend")
};

USTRUCT(BlueprintType)
struct FInventoryStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY(EditAnywhere)
	FName WpnName;

	UPROPERTY(EditAnywhere)
	int WpnRating;

	UPROPERTY(EditAnywhere)
	EItemType WpnAmmoType;

	UPROPERTY(EditAnywhere)
	ERarity WpnRarity;

	//UPROPERTY(EditAnywhere)
	//TObjectPtr<Texture> WpnIcon;

	
};