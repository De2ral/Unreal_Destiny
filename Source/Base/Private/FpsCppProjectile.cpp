// Copyright Epic Games, Inc. All Rights Reserved.

#include "FpsCppProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Net/UnrealNetwork.h" 


#include "Kismet/GameplayStatics.h"

AFpsCppProjectile::AFpsCppProjectile() 
{
    PrimaryActorTick.bCanEverTick = true;

    // 네트워크 복제 설정
    bReplicates = true; 
    SetReplicateMovement(true);



	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
			// set up a notification for when this component hits something blocking
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
    CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Block);
    CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);  // Added response for Pawn

	
	//CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AFpsCppProjectile::OnOverlapBegin);
	CollisionComp->OnComponentHit.AddDynamic(this, &AFpsCppProjectile::OnHit);

	// Players can't walk on it
	//CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	//CollisionComp->CanCharacterStepUpOn = ECB_No;
    CollisionComp->SetIsReplicated(true); // 콜리전 컴포넌트 복제 설정
	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	
	// Die after 3 seconds by default
	InitialLifeSpan = 10.0f;

	// 메쉬 컴포넌트 생성 및 설정


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	Mesh->SetupAttachment(RootComponent);
    Mesh->SetIsReplicated(true); // 메쉬 컴포넌트도 복제되도록 설정

	// 메쉬의 충돌 설정을 비활성화
	//Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
    Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Block);
    Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);

	//Mesh->OnComponentBeginOverlap.AddDynamic(this, &AFpsCppProjectile::OnOverlapBegin);
	Mesh->OnComponentHit.AddDynamic(this, &AFpsCppProjectile::OnHit);

	Tags.Add(FName("Bullets"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_B.P_Explosion_Big_B'"));
    if (ParticleAsset.Succeeded())
    {
        HitFlash_Launcher = ParticleAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset1(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_A.P_Explosion_Big_A'"));
    if (ParticleAsset.Succeeded())
    {
        HitFlash = ParticleAsset1.Object;
    }

   static ConstructorHelpers::FObjectFinder<UNiagaraSystem> TrailNiagaraAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/MegaMagicVFXBundle/VFX/MagicAuraVFX/VFX/LightningField/Systems/N_LightningField.N_LightningField'"));
    if (TrailNiagaraAsset.Succeeded())
    {
        TrailEffect = TrailNiagaraAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> TrailNiagaraAsset1(TEXT("/Script/Niagara.NiagaraSystem'/Game/MegaMagicVFXBundle/VFX/MagicAuraVFX/VFX/RingOfFlames/Systems/N_RingOfFlames.N_RingOfFlames'"));
    if (TrailNiagaraAsset1.Succeeded())
    {
        TrailEffect_Launcher = TrailNiagaraAsset1.Object;
    }


}

void AFpsCppProjectile::BeginPlay()
{
    Super::BeginPlay();
    
}

void AFpsCppProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void AFpsCppProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AFpsCppProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("OnHit."));
    UE_LOG(LogTemp, Warning, TEXT("Damage : %f"), Damage);

    if (OtherActor && OtherActor != this)
    {
       if (bExplodeOnImpact)
        {
            // 폭발 데미지를 적용
            UGameplayStatics::ApplyRadialDamage(
                this,
                Damage,
                GetActorLocation(),
                ExplosionRadius,
                UDamageType::StaticClass(),
                TArray<AActor*>(),
                this,
                GetInstigatorController(),
                true
            );

            if (HitFlash_Launcher)
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFlash_Launcher, Hit.Location,FRotator(0.0f, 0.0f, 0.0f), FVector(2.0f, 2.0f, 2.0f));
            Destroy();
        }
        else
        {
            AController* InstigatorController = GetInstigatorController();
            UGameplayStatics::ApplyDamage(
                OtherActor,
                Damage,
                InstigatorController,
                this,
                UDamageType::StaticClass()
            );

            if (HitFlash)
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFlash, Hit.Location,FRotator(0.0f, 0.0f, 0.0f), FVector(0.25f, 0.25f, 0.25f));
            Destroy();
        }
    }
}

void AFpsCppProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
    
}



void AFpsCppProjectile::FireInDirection(const FVector& ShootDirection)
{
    if (ProjectileMovement)
    {
        ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
    }
}

void AFpsCppProjectile::SetProjectileMesh(UStaticMesh* NewMesh)
{
	if (NewMesh)
	{
		Mesh->SetStaticMesh(NewMesh);
	}
}

void AFpsCppProjectile::SetProjectile(UStaticMesh* NewMesh, float speed, float damage)
{
	SetProjectileMesh(NewMesh);
	ProjectileMovement->InitialSpeed = speed;
	ProjectileMovement->MaxSpeed = speed;
	Damage = damage;
}
void AFpsCppProjectile::AttachTrailEffect(bool isRifle)
{
    if(isRifle)
    {
        if (TrailEffect)
        {
            UNiagaraComponent* TrailNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
                TrailEffect,
                Mesh,
                NAME_None,
                FVector::ZeroVector,
                FRotator::ZeroRotator,
                EAttachLocation::KeepRelativeOffset,
                true
            );
            TrailNiagaraComponent->SetRelativeScale3D(FVector(2.0f, 0.25f, 0.25f)); // 크기 조절 (옵션)
        }
    }

    else
    {
        if (TrailEffect_Launcher)
        {
            UNiagaraComponent* TrailNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
                TrailEffect_Launcher,
                Mesh,
                NAME_None,
                FVector::ZeroVector,
                FRotator::ZeroRotator,
                EAttachLocation::KeepRelativeOffset,
                true
            );
            TrailNiagaraComponent->SetRelativeScale3D(FVector(2.0f, 0.25f, 0.25f)); // 크기 조절 (옵션)
        }
    }
}

void AFpsCppProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Mesh 컴포넌트의 복제 설정
    //DOREPLIFETIME(AFpsCppProjectile, Mesh);

    // 필요시 다른 변수도 복제 설정 가능
    //DOREPLIFETIME(AFpsCppProjectile, Damage);
}