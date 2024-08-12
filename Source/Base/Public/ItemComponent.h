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


UCLASS()
class BASE_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* ItemCollider;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UMaterial* SpecAmmoMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UMaterial* NormalAmmoMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UMaterial* RefAmmoMaterial;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	EItemType ThisItemType;

		
};
