// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "InventorySystem.h"
#include "CPP_DropItem.generated.h"

UENUM(BlueprintType)
enum class EDropItemType : uint8
{
	Ammo UMETA(DisplayName = "Ammo"),
	RefAmmo UMETA(DisplayName = "RefAmmo"),
	SpecAmmo UMETA(DisplayName = "SpecAmmo"),
	Weapon UMETA(DisplayName = "Weapon")
};

UCLASS()
class BASE_API ACPP_DropItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_DropItem();

protected:
	// Called when the game starts or when spawned
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
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	EDropItemType ThisItemType;

};
