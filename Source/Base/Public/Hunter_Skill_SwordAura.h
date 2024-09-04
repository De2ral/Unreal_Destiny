// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hunter_Skill_SwordAura.generated.h"

UCLASS()
class BASE_API AHunter_Skill_SwordAura : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHunter_Skill_SwordAura();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* SwordAuraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* SwordAuraThunderParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* ExplodeParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float Speed = 2000.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DamageAmount = 70.f;

	UPROPERTY(VisibleAnywhere)
	float DestroyDelay = 5.f;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
    class UProjectileMovementComponent* ProjectileMovement;

    FVector SwordAuraDirection;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    void ApplyDamage();
	void PlayExlodeParticle();
	void DestroyAura();

	void SetSwordAuraDirection(const FVector& Direction);
};
