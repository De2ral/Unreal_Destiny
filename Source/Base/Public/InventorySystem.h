// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DestinyFPSBase.h"
#include "Kismet/GameplayStatics.h"
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

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int CurrAmmo = 0;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int MaxAmmo = 480;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int CurrSpecialAmmo = 0;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int MaxSpecialAmmo = 60;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int CurrRefAmmo = 0;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int MaxRefAmmo = 180;


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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//inline int GetCurrAmmo() {return CurrAmmo; }

	//inline int GetMaxAmmo() {return MaxAmmo; }

	//inline int CurrSpecialAmmo() {return CurrSpecialAmmo; }

	//inline int MaxSpecialAmmo() {return MaxSpecialAmmo; }

	//inline int CurrRefAmmo() {return CurrRefAmmo; }

	//inline int MaxRefAmmo() {return MaxRefAmmo; }

		
};
