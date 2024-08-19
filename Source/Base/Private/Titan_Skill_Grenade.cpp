// Fill out your copyright notice in the Description page of Project Settings.


#include "Titan_Skill_Grenade.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "DestinyFPSBase.h"

// Sets default values
ATitan_Skill_Grenade::ATitan_Skill_Grenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/ParagonDrongo/FX/Meshes/Heroes/Drongo/SM_Drongo_Grenade_FX_Body02.SM_Drongo_Grenade_FX_Body02'"));
	if (MeshAsset.Succeeded())
		Mesh->SetStaticMesh(MeshAsset.Object);
	RootComponent = Mesh;


	if(!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->InitialSpeed = 4000.0f;
		ProjectileMovementComponent->MaxSpeed = 4000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 1.f;
	}


	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonDrongo/FX/Particles/Abilities/Grenade/FX/P_Drongo_Grenade_Explode.P_Drongo_Grenade_Explode'"));
	if (ParticleAsset.Succeeded())
		ExplodeParticle = ParticleAsset.Object;


	ExplodeCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ExplodeCollider"));
	ExplodeCollider->SetupAttachment(RootComponent);
	ExplodeCollider->SetGenerateOverlapEvents(true);

	Mesh->OnComponentHit.AddDynamic(this, &ATitan_Skill_Grenade::OnHit);
}

// Called when the game starts or when spawned
void ATitan_Skill_Grenade::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATitan_Skill_Grenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATitan_Skill_Grenade::SetThrowDirection(FVector Direction)
{
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}

void ATitan_Skill_Grenade::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
                         FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->IsA(ADestinyFPSBase::StaticClass()))
		return;

	this->PlayExplodeParticleSystem();
	this->ApplyDamageToActorInsideExplodeCollider();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATitan_Skill_Grenade::DestroyGrenade, 0.01f, false);
}

void ATitan_Skill_Grenade::DestroyGrenade()
{
	if (this)
		this->Destroy();
}

void ATitan_Skill_Grenade::PlayExplodeParticleSystem()
{
    if (ExplodeParticle)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ExplodeParticle,
            GetActorLocation(),
            GetActorRotation(),
			(FVector)((1.5F))
        );
    }
}

void ATitan_Skill_Grenade::ApplyDamageToActorInsideExplodeCollider()
{
	TArray<AActor*> OverlappingActors;
	ExplodeCollider->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor->IsA(ADestinyFPSBase::StaticClass()))
			UGameplayStatics::ApplyDamage(Actor, GrenadeDamage, GetInstigatorController(), this, nullptr);
	}
}
