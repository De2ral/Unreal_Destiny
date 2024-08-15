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

}
// Called when the game starts
void UInventorySystem::BeginPlay()
{
	Super::BeginPlay();

	WeaponArray.SetNum(MaxInvenSize);

	for(int i = 0; i < 4;i++)
	{
		AddWeaponToInventory();
	}

}


void UInventorySystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UInventorySystem::AddWeaponToInventory()
{	

	if(WpnArrayIndex >= MaxInvenSize) return;

	uint8 randomSeed;
	randomSeed = FMath::RandRange(2,4);
	FName PistolName;
	switch (randomSeed)
	{
	case 2:
		PistolName = FName("Pistol");
		break;
	case 3:
		PistolName = FName("Pistol2");
		break;
	case 4:
		PistolName = FName("Pistol3");
		break;
	default:
		break;
	}
	WeaponArray[WpnArrayIndex].GunName = PistolName;
	WpnArrayIndex++;


}


