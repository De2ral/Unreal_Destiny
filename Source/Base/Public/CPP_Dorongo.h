// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_Dorongo.generated.h"

UCLASS()
class BASE_API ACPP_Dorongo : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_Dorongo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void PerformLineTrace();
	FVector StartPoint;
	FVector EndPoint;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Enemy")
	float HP;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Enemy")
	float MaxHP = 100;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Enemy")
	bool isDead = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Enemy")
	bool isAttack = false;

	//UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Enemy")
	//bool bHit;

public:
    UFUNCTION(BlueprintCallable, Category = "HP")
    void TakeDamage1(float DamageAmount);

	//UPROPERTY(EditAnywhere,BlueprintReadWrite)
    //UStaticMeshComponent* PhysicsMesh;
	UFUNCTION(BlueprintCallable, Category="Physics")
    void EnablePhysicsSimulation();


};
