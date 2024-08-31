// Fill out your copyright notice in the Description page of Project Settings.


#include "Warlock_Melee_Fireball.h"
#include "DestinyFPSBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWarlock_Melee_Fireball::AWarlock_Melee_Fireball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic")); 
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AWarlock_Melee_Fireball::OnOverlapBegin);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = Speed;
    ProjectileMovement->MaxSpeed = Speed;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FireballAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_fireBall.P_ky_fireBall'"));
	if (FireballAsset.Succeeded())
		FireballParticle = FireballAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplodeAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_explosion.P_ky_explosion'"));
	if (ExplodeAsset.Succeeded())
		ExplodeParticle = ExplodeAsset.Object;
}

// Called when the game starts or when spawned
void AWarlock_Melee_Fireball::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::SpawnEmitterAttached(
		FireballParticle,
		RootComponent,
		NAME_None,
		FVector::ZeroVector, 
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true
	);
}

// Called every frame
void AWarlock_Melee_Fireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + (FireballDirection * Speed * DeltaTime);
    SetActorLocation(NewLocation);
}

void AWarlock_Melee_Fireball::SetFireballDirection(const FVector& Direction)
{
    FireballDirection = Direction;
}

void AWarlock_Melee_Fireball::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && !(OtherActor->IsA(ADestinyFPSBase::StaticClass())) && !(OtherActor->IsA(AWarlock_Melee_Fireball::StaticClass())))
	{
		ApplyDamage();
		PlayExlodeParticle();
		Destroy();
	}
}

void AWarlock_Melee_Fireball::ApplyDamage()
{
    // 파이어볼의 스피어 콜리전 범위 내에 있는 모든 액터에 대해 데미지 적용
    TArray<AActor*> OverlappingActors;
    if (CollisionComponent)
    {
        CollisionComponent->GetOverlappingActors(OverlappingActors);
        
        for (AActor* Actor : OverlappingActors)
        {
            UGameplayStatics::ApplyDamage(Actor, DamageAmount, GetInstigatorController(), this, nullptr);
        }
    }
}

void AWarlock_Melee_Fireball::PlayExlodeParticle()
{
	FVector ParticleSpawnLocation = this->GetActorLocation();
	FRotator ParticleSpawnRotation = this->GetActorRotation();
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		ExplodeParticle,
		ParticleSpawnLocation,
		ParticleSpawnRotation,
		(FVector)((1.5F))
	);
}