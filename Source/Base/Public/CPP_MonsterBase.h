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
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite, Category = "Enemy")
	float HP;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Enemy")
	float MaxHP = 100;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite, Category = "Enemy")
	float DamageValue;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite, Category = "Enemy")
	bool isDead = false;
	
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite, Category = "Combat")
	bool isAttack;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Critical")
    class USphereComponent* CriticalHitBox;

	bool bCanTakeDamage = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Critical")
	bool bIsCriticalHit = false;

	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "Critical")
	bool bIsCriticalTextColor = false;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DamageCooldownTime = 0.1f;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite, Category = "Item")
	int32 MinItemValue = 1;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite, Category = "Item")
	int32 MaxItemValue = 10;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
    void OnRep_HP();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, WithValidation)
    void ServerTakeDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "HP")
	virtual float TakeDamage(float DamageAmount, 
    FDamageEvent const& DamageEvent, 
    AController* EventInstigator, 
    AActor* DamageCauser)override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastTakeDamage(float DamageAmount);

	void ResetDamageCoolDown();

	//UPROPERTY(EditAnywhere,BlueprintReadWrite)
    //UStaticMeshComponent* PhysicsMesh;
	UFUNCTION(BlueprintCallable, Category = "Physics")
    void EnablePhysicsSimulation();

	UFUNCTION(NetMulticast, Reliable,BlueprintCallable, Category = "Spawn")
	void ItemDrop(int32 ItemCount);



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TArray<TSubclassOf<AActor>> ItemsToSpawn;


	UFUNCTION()
    void OnCriticalHitBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                                 bool bFromSweep, const FHitResult& SweepResult);


	FTimerHandle DestroyTimer;

	void DestroyActor();

};
