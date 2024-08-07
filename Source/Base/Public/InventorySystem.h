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


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* InventoryAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputMappingContext* InventoryMappingContext;

	void InvenOpenClose();

	void AddMapping();

	UPROPERTY(EditDefaultsOnly)
	bool bIsInvenOpen = false;

	UPROPERTY(EditDefaultsOnly)
	int CurrAmmo = 0;

	UPROPERTY(EditDefaultsOnly)
	int CurrMaxAmmo = 0;

	UPROPERTY(EditDefaultsOnly)
	int CurrSpecialAmmo = 0;

	UPROPERTY(EditDefaultsOnly)
	int CurrMaxSpecialAmmo = 0;

	UPROPERTY(EditDefaultsOnly)
	int CurrRefAmmo = 0;

	UPROPERTY(EditDefaultsOnly)
	int CurrMaxRefAmmo = 0;

	


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
		
};
