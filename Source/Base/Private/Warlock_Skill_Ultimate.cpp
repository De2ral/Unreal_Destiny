// Fill out your copyright notice in the Description page of Project Settings.


#include "Warlock_Skill_Ultimate.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWarlock_Skill_Ultimate::AWarlock_Skill_Ultimate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> StartAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_lightning2_2.P_ky_lightning2_2'"));
	if (StartAsset.Succeeded())
		StartParticle = StartAsset.Object;	

	static ConstructorHelpers::FObjectFinder<UParticleSystem> AuraAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Ultimate/FX/P_Gideon_Ultimate_Cast.P_Gideon_Ultimate_Cast'"));
	if (AuraAsset.Succeeded())
		AuraParticle = AuraAsset.Object;	
}

// Called when the game starts or when spawned
void AWarlock_Skill_Ultimate::BeginPlay()
{
	Super::BeginPlay();
	
	if (StartParticle)
    {
		FVector ParticleSpawnLocation = GetActorLocation();
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            StartParticle,
            ParticleSpawnLocation,
            FRotator(0.f, 0.f, 0.f),
			(FVector)((1.F))
        );
    }

	if (AuraParticle)
    {
		FVector ParticleSpawnLocation = GetActorLocation();
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            AuraParticle,
            ParticleSpawnLocation,
            FRotator(0.f, 0.f, 0.f),
			(FVector)((1.F))
        );
    }

}

// Called every frame
void AWarlock_Skill_Ultimate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

