// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

// Sets default values for this component's properties
UInventorySystem::UInventorySystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	WeaponArray.SetNum(MaxInvenSize);
	//EQWeaponArray.SetNum(3);

	static ConstructorHelpers::FObjectFinder<UTexture2D>GunImageObject(TEXT("/Script/Engine.Texture2D'/Engine/EditorResources/S_Pawn.S_Pawn'"));
	GunImg = GunImageObject.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>ProjMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	
	for(int i = 0; i < MaxInvenSize; i++)
	{
		WeaponArray[i].GunImage = GunImg;
		WeaponArray[i].BulletType = BulletTypeList::REGULAR;
		WeaponArray[i].AutoFire = false;
		WeaponArray[i].Max_capacity = 0;
		WeaponArray[i].FireRate = 0.0f;
		WeaponArray[i].Linetracing = false;
		WeaponArray[i].Rebound = 0.0f;
		WeaponArray[i].ProjectileSpeed = 0.0f;
		WeaponArray[i].ProjectileMesh = ProjMesh.Object;
		WeaponArray[i].GunName = FName("aaaa");
		WeaponArray[i].GunDamage = 0.0f;
		WeaponArray[i].GunType = GunTypeList::PISTOL;
		WeaponArray[i].GunModelPath = "/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun";
		WeaponArray[i].AimLocation = 0.0f;
		WeaponArray[i].AimRotation = 0.0f;
		WeaponArray[i].CameraZoom = 0.0f;
	}



}
// Called when the game starts
void UInventorySystem::BeginPlay()
{
	Super::BeginPlay();

	for(int i = 0; i < 4;i++)
	{
		AddWeaponToInventory();
	}

	
}


void UInventorySystem::AddWeaponToInventory()
{	

	uint8 randomSeed = FMath::RandRange(2, 6);
	FName wpnName;
	switch (randomSeed)
	{
	case 2:
	    wpnName = FName("Pistol1");
	    break;
	case 3:
	    wpnName = FName("Pistol2");
	    break;
	case 4:
	    wpnName = FName("Pistol3");
	    break;
	case 5:
	    wpnName = FName("Rifle3");
	    break;
	case 6:
	    wpnName = FName("Sniper3");
	    break;
	default:
	    break;
	}

	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Weapon/WeaponData.WeaponData'"));

	if(DataTable)
	{
    	FGunInfo* Row = DataTable->FindRow<FGunInfo>(wpnName, TEXT(""));

		//행을 가져왔다 그리고 넣어줬다. (기초적인 성분들이 있는 상황)
		WeaponArray[WpnArrayIndex] = *Row;

		float CurrentFireRate = WeaponArray[WpnArrayIndex].FireRate;
		float CurrentTotalDamage = (60.0f / CurrentFireRate) * WeaponArray[WpnArrayIndex].GunDamage;

		// 습득한 무기가 제일 전투력이 높은 상황 -> 그러면 전투력 이걸로 바꾸고 아무것도 하지마
		if (CurrentTotalDamage > MaxTotalDamage)
        {
            MaxTotalDamage = CurrentTotalDamage;
        }
		//습득한 무기가 기준보다 전투력이 낮다! -> 높혀줘야지
		else if (CurrentTotalDamage <= MaxTotalDamage)
		{
			  // 새로운 무기의 총 공격력이 기존 무기보다 높도록 GunDamage 설정
    		float NewTotalDamage = MaxTotalDamage + FMath::RandRange(10.0f, 40.0f); // 총 데미지를 기존보다 높게 설정
    		float NewGunDamage = (NewTotalDamage * CurrentFireRate) / 60.0f;
			WeaponArray[WpnArrayIndex].GunDamage = NewGunDamage;
		}
	}

	// 무기 배열 인덱스 증가
	WpnArrayIndex++;

}
