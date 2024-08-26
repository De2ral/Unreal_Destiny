#include "CPP_BossMissile.h"
#include "Math/Vector.h"
#include "Math/UnrealMathUtility.h"

ACPP_BossMissile::ACPP_BossMissile()
{
    PrimaryActorTick.bCanEverTick = true;

    // MeshComponent 초기화
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // ProjectileMovementComponent 초기화
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->bAutoActivate = false; // 기본적으로 비활성화
}

void ACPP_BossMissile::BeginPlay()
{
    Super::BeginPlay();
}

void ACPP_BossMissile::LaunchProjectile(FVector LaunchDirection, float LaunchSpeed)
{
    if (ProjectileMovementComponent)
    {
        // 발사체의 초기 속도 설정
        ProjectileMovementComponent->Velocity = LaunchDirection * LaunchSpeed;
        ProjectileMovementComponent->Activate(); // 발사체 이동 컴포넌트 활성화
    }
}

FVector ACPP_BossMissile::CalculateImpactPoint(float Gravity) const
{
   if (!ProjectileMovementComponent)
    {
        return FVector::ZeroVector; // 컴포넌트가 없는 경우
    }

    FVector InitialVelocity = ProjectileMovementComponent->Velocity;
    FVector InitialPosition = GetActorLocation();

    // 수직 속도 성분
    float VelocityZ = InitialVelocity.Z;

    // 수평 속도 성분
    FVector HorizontalVelocity = FVector(InitialVelocity.X, InitialVelocity.Y, 0);

    // 포탄이 공중에 머무는 총 시간 계산
    float TimeOfFlight = VelocityZ / Gravity + FMath::Sqrt(FMath::Square(VelocityZ) / FMath::Square(Gravity) + (2 * InitialPosition.Z) / Gravity);

    // 수평 거리 계산
    FVector HorizontalDisplacement = HorizontalVelocity * TimeOfFlight;

    // 도착 지점 계산
    FVector ImpactPoint = InitialPosition + HorizontalDisplacement;

    return ImpactPoint;
    
}
