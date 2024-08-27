// Copyright Epic Games, Inc. All Rights Reserved.

#include "FpsCppProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "Kismet/GameplayStatics.h"

AFpsCppProjectile::AFpsCppProjectile() 
{
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
	InitialLifeSpan = 3.0f;

	// 메쉬 컴포넌트 생성 및 설정
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	Mesh->SetupAttachment(RootComponent);

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
        HitFlash = ParticleAsset.Object;
    }
}
void AFpsCppProjectile::BeginPlay()
{
    Super::BeginPlay();

	
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

            if (HitFlash)
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFlash, Hit.Location);

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

            Destroy();
        }
    }

	// Only add impulse and destroy projectile if we hit a physics
	//if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	//{
	//	OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	//	
	//	Destroy();
	//}
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