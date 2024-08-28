// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Grenade.generated.h"

UCLASS()
class BASE_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* ExplodeParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float ExplosionRadius = 300.f;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
               FVector NormalImpulse, const FHitResult& Hit);

	void PlayExplodeParticleSystem();
	void ApplyRadialDamageToActor();
	void SetThrowDirection(FVector Direction);
	void DestroyGrenade();

private:
	float GrenadeDamage = 50.f;
	FTimerHandle TimerHandle;
};