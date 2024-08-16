// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "InventorySystem.h"
#include "ItemComponent.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Ammo UMETA(DisplayName = "Ammo"),
	RefAmmo UMETA(DisplayName = "RefAmmo"),
	SpecAmmo UMETA(DisplayName = "SpecAmmo"),
	Weapon UMETA(DisplayName = "Weapon")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASE_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* ItemCollider;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere)
	class UMaterial* SpecAmmoMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterial* NormalAmmoMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterial* RefAmmoMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterial* WeaponItemMaterial;

	UPROPERTY(VisibleAnywhere)
	int PickedWeaponIndex = 0;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	EItemType ThisItemType;

		
};
