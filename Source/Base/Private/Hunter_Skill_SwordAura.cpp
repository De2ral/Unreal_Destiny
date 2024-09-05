// Fill out your copyright notice in the Description page of Project Settings.

#include "Hunter_Skill_SwordAura.h"
#include "TimerManager.h"

// Sets default values
AHunter_Skill_SwordAura::AHunter_Skill_SwordAura()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic")); 
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AHunter_Skill_SwordAura::OnOverlapBegin);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = Speed;
    ProjectileMovement->MaxSpeed = Speed;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> SwordAuraAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Combat_Base/WeaponCombo/P_Combo_Finish_Fire_01.P_Combo_Finish_Fire_01'"));
	if (SwordAuraAsset.Succeeded())
		SwordAuraParticle = SwordAuraAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> SwordAuraThunderAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKwang/FX/Particles/Abilities/Sword/FX/P_Kwang_Sword_Attach.P_Kwang_Sword_Attach'"));
	if (SwordAuraThunderAsset.Succeeded())
		SwordAuraThunderParticle = SwordAuraThunderAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplodeAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKwang/FX/Particles/Abilities/LightStrike/FX/P_LightStrikeImpact_2.P_LightStrikeImpact_2'"));
	if (ExplodeAsset.Succeeded())
		ExplodeParticle = ExplodeAsset.Object;
}

// Called when the game starts or when spawned
void AHunter_Skill_SwordAura::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("회전 X : %f, Y : %f, Z : %f"), GetActorRotation().Vector().X, GetActorRotation().Vector().Y, GetActorRotation().Vector().Z));
	
	UGameplayStatics::SpawnEmitterAttached(
		SwordAuraParticle,
		RootComponent,
		NAME_None,
		FVector::ZeroVector, 
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true
	);

	UGameplayStatics::SpawnEmitterAttached(
		SwordAuraThunderParticle,
		RootComponent,
		NAME_None,
		FVector::ZeroVector, 
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true
	);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AHunter_Skill_SwordAura::DestroyAura, DestroyDelay, false);
}

// Called every frame
void AHunter_Skill_SwordAura::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + (SwordAuraDirection * Speed * DeltaTime);
    SetActorLocation(NewLocation);
}

void AHunter_Skill_SwordAura::SetSwordAuraDirection(const FVector& Direction)
{
    SwordAuraDirection = Direction;
}

void AHunter_Skill_SwordAura::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && !(OtherActor->IsA(ADestinyFPSBase::StaticClass())) && !(OtherActor->IsA(AHunter_Skill_SwordAura::StaticClass())))
	{
		ApplyDamage();
		//PlayExlodeParticle();
		Destroy();
	}
}

void AHunter_Skill_SwordAura::ApplyDamage()
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

void AHunter_Skill_SwordAura::PlayExlodeParticle()
{
	FVector ParticleSpawnLocation = this->GetActorLocation();
	FRotator ParticleSpawnRotation = this->GetActorRotation();
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		ExplodeParticle,
		ParticleSpawnLocation,
		ParticleSpawnRotation,
		(FVector)((0.5F))
	);
}

void AHunter_Skill_SwordAura::DestroyAura()
{
	Destroy();
}