// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_MonsterBase.generated.h"

UCLASS()
class BASE_API ACPP_MonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_MonsterBase();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Enemy")
	float HP;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Enemy")
	float MaxHP = 100;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Enemy")
	float DamageValue;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Enemy")
	bool isDead = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Combat")
	bool isAttack;	

	bool bCanTakeDamage = true;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DamageCooldownTime = 0.1f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Item")
	int32 MinItemValue = 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Item")
	int32 MaxItemValue = 10;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "HP")
	virtual float TakeDamage(float DamageAmount, 
    FDamageEvent const& DamageEvent, 
    AController* EventInstigator, 
    AActor* DamageCauser)override;

	void ResetDamageCoolDown();

	//UPROPERTY(EditAnywhere,BlueprintReadWrite)
    //UStaticMeshComponent* PhysicsMesh;
	UFUNCTION(BlueprintCallable, Category = "Physics")
    void EnablePhysicsSimulation();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void ItemDrop(int32 ItemCount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TArray<TSubclassOf<AActor>> ItemsToSpawn;

};
