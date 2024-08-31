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

	if(WpnArrayIndex >= MaxInvenSize) return;

	uint8 randomSeed;
	randomSeed = FMath::RandRange(2,6);
	FName PistolName;
	switch (randomSeed)
	{
	case 2:
		PistolName = FName("Pistol1");
		break;
	case 3:
		PistolName = FName("Pistol2");
		break;
	case 4:
		PistolName = FName("Pistol3");
		break;
	case 5:
		PistolName = FName("Rifle3");
		break;
	case 6:
		PistolName = FName("Sniper3");
		break;
	default:
		break;
	}
	WeaponArray[WpnArrayIndex].GunName = PistolName;
	WpnArrayIndex++;


}


