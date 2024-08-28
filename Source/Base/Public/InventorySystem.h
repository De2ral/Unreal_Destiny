// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DestinyFPSBase.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponDataManage.h"
#include "InventorySystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASE_API UInventorySystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventorySystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void AddMapping(ADestinyFPSBase* TargetCharacter);

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int CurrAmmo = 0;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int MaxAmmo = 480;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int CurrSpecialAmmo = 0;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int MaxSpecialAmmo = 30;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int CurrRefAmmo = 0;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int MaxRefAmmo = 180;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int WpnArrayIndex = 0;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int MaxInvenSize = 30;
	
	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* GunImg;

	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// class UInputAction* InventoryAction;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// class UInputMappingContext* InventoryMappingContext;

	// void InvenOpenClose();

	// void AddMapping();

	// UPROPERTY(EditDefaultsOnly)
	// bool bIsInvenOpen = false;

public:	
	// Called every frame
	
	UFUNCTION()
	void AddCurrAmmo(int value) { CurrAmmo += value; }

	UFUNCTION()
	void MinusCurrAmmo(int value) { CurrAmmo -= value; }

	UFUNCTION()
	void AddCurrSpecialAmmo(int value) { CurrSpecialAmmo += value; }

	UFUNCTION()
	void MinusCurrSpecialAmmo(int value) { CurrSpecialAmmo -= value; }

	UFUNCTION()
	void AddCurrRefAmmo(int value) { CurrRefAmmo += value; }

	UFUNCTION()
	void MinusCurrRefAmmo(int value) { CurrRefAmmo -= value; }

	UFUNCTION()
	int GetCurrAmmo() { return CurrAmmo; }

	UFUNCTION()
	int GetCurrSpecialAmmo() { return CurrSpecialAmmo; }

	UFUNCTION()
	int GetCurrRefAmmo() { return CurrRefAmmo; }

	UFUNCTION()
	void AddWeaponToInventory();

	bool bIsAmmoFull() {return (CurrAmmo > MaxAmmo) ? true : false; }

	bool bIsSpecAmmoFull() {return (CurrSpecialAmmo > MaxSpecialAmmo) ? true : false; }

	bool bIsRefAmmoFull() {return (CurrRefAmmo > MaxRefAmmo) ? true : false; }

	UPROPERTY(BlueprintReadWrite)
	TArray<FGunInfo> WeaponArray;

	//UPROPERTY(BlueprintReadWrite)
	//TArray<FGunInfo> EQWeaponArray;



private:
	ADestinyFPSBase* Character;

	void InvenOpenClose();
		
};
