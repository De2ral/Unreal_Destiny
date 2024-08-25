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
    RIFLE UMETA(DisplayName = "Rifle"),
    SHOTGUN UMETA(DisplayName = "Shotgun"),
	SNIPER UMETA(DisplayName = "Sniper"),
	LAUNCHER UMETA(DisplayName = "Launcher")
};

UENUM(BlueprintType)
enum class BulletTypeList : uint8
{
	REGULAR UMETA(DisplayName = "Regular"),
	SPECIAL UMETA(DisplayName = "Special"),
	REINFORCE UMETA(DisplayName = "Reinforce")
};


USTRUCT(BlueprintType)
struct FGunInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	FGunInfo(){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName GunName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	GunTypeList GunType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	BulletTypeList BulletType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GunDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Max_capacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rebound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Linetracing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AutoFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GunModelPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* GunImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimRotation;
};
