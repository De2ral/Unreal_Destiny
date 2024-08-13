// Copyright Epic Games, Inc. All Rights Reserved.

#include "FpsCppProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

#include "Kismet/GameplayStatics.h"

AFpsCppProjectile::AFpsCppProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	//CollisionComp->OnComponentHit.AddDynamic(this, &AFpsCppProjectile::OnHit);		// set up a notification for when this component hits something blocking
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AFpsCppProjectile::OnOverlapBegin);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

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
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Tags.Add(FName("Bullets"));
}

void AFpsCppProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		
		Destroy();
	}
}

void AFpsCppProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // OtherActor가 유효하고, 자기 자신이 아닌 경우에만 데미지를 가합니다.

	UE_LOG(LogTemp, Warning, TEXT("OnOverlapBegin."));
    if (OtherActor && OtherActor != this)
    {
        AController* InstigatorController = GetInstigatorController();

        // 데미지 적용
        UGameplayStatics::ApplyDamage(
            OtherActor,              
            Damage,            
            InstigatorController,    
            this,                   
            UDamageType::StaticClass()
        );

        // 발사체를 파괴합니다.
        Destroy();
    }
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