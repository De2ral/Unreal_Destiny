// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "DestinyFPSBase.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/ParagonDrongo/FX/Meshes/Heroes/Drongo/SM_Drongo_Grenade_FX_Body02.SM_Drongo_Grenade_FX_Body02'"));
	if (MeshAsset.Succeeded())
		Mesh->SetStaticMesh(MeshAsset.Object);
	RootComponent = Mesh;


	if(!ProjectileMovementComponent)
	{
		if(!IsProjectile)
		{
			ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
			ProjectileMovementComponent->InitialSpeed = 4000.0f;
			ProjectileMovementComponent->MaxSpeed = 4000.0f;
			ProjectileMovementComponent->bRotationFollowsVelocity = true;
			ProjectileMovementComponent->bShouldBounce = true;
			ProjectileMovementComponent->Bounciness = 0.3f;
			ProjectileMovementComponent->ProjectileGravityScale = 1.f;
		}

		else
		{
			ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
			ProjectileMovementComponent->InitialSpeed = 4000.0f;
			ProjectileMovementComponent->MaxSpeed = 4000.0f;
			ProjectileMovementComponent->bRotationFollowsVelocity = true;
			ProjectileMovementComponent->bShouldBounce = false;
			ProjectileMovementComponent->ProjectileGravityScale = 0.f;
		}
	}


	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonDrongo/FX/Particles/Abilities/Grenade/FX/P_Drongo_Grenade_Explode.P_Drongo_Grenade_Explode'"));
	if (ParticleAsset.Succeeded())
		ExplodeParticle = ParticleAsset.Object;

	Mesh->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenade::SetThrowDirection(FVector Direction)
{
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}

void AGrenade::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
                         FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->IsA(ADestinyFPSBase::StaticClass()))
		return;

	this->PlayExplodeParticleSystem();
	this->ApplyRadialDamageToActor();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGrenade::DestroyGrenade, 0.01f, false);
}

void AGrenade::DestroyGrenade()
{
	if (this)
		this->Destroy();
}

void AGrenade::PlayExplodeParticleSystem()
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

void AGrenade::ApplyRadialDamageToActor()
{
	UGameplayStatics::ApplyRadialDamage(
		this,
		GrenadeDamage,
		GetActorLocation(),
		ExplosionRadius,
		UDamageType::StaticClass(),
		TArray<AActor*>(),
		this,
		GetInstigatorController(),
		true
	);
}
