// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Warlock_Melee_Fireball.generated.h"

UCLASS()
class BASE_API AWarlock_Melee_Fireball : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarlock_Melee_Fireball();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* FireballParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* ExplodeParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float Speed = 1000.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DamageAmount = 50.0f;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
    class UProjectileMovementComponent* ProjectileMovement;

    FVector FireballDirection;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    void ApplyDamage();
	void PlayExlodeParticle();

	void SetFireballDirection(const FVector& Direction);
};
