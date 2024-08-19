// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"

#include "WeaponDataManage.generated.h"

UENUM(BlueprintType)
enum class GunTypeList : uint8
{
	PISTOL UMETA(DisplayName = "Pistol"),
    SHOTGUN UMETA(DisplayName = "Shotgun"),
    RIFLE UMETA(DisplayName = "Rifle")
};

UENUM(BlueprintType)
enum class BulletTypeList : uint8
{
	REGULAR UMETA(DisplayName = "Regular"),
	SPECIAL UMETA(DisplayName = "Special"),
	REINFORCE UMETA(DisplayName = "Reinforce")
};

UENUM(BlueprintType)
enum class FireTypeList : uint8
{
	SINGLE UMETA(DisplayName = "Single"),
	BURST UMETA(DisplayName = "Burst"),
	CONTINU UMETA(DisplayName = "Continu")
};

USTRUCT(BlueprintType)
struct FGunInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	FGunInfo(){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName GunID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName GunName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	GunTypeList GunType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	BulletTypeList BulletType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FireTypeList FireType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GunDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rebound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AutoFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GunModelPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* GunImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Linetracing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ProjectileMesh;
};
