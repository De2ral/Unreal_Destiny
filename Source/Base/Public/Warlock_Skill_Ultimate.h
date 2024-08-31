// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Warlock_Skill_Ultimate.generated.h"

UCLASS()
class BASE_API AWarlock_Skill_Ultimate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarlock_Skill_Ultimate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
    class USphereComponent* AuraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* StartParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* AuraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealAmount = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	float HealInterval = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float lifeTime = 30.f;

	class FTimerHandle HealTimerHandle;
	class FTimerHandle DestroyTimerHandle;

	void ApplyHealing();
	void DestroySkill();
}
