#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CPP_BossMissile.generated.h"

UCLASS()
class BASE_API ACPP_BossMissile : public AActor
{
    GENERATED_BODY()
    
public:    
    ACPP_BossMissile();

protected:
    virtual void BeginPlay() override;

public:    
    // 발사체의 메쉬 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    // 발사체의 이동 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UProjectileMovementComponent* ProjectileMovementComponent;

    // 발사체를 발사하는 함수
    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void LaunchProjectile(FVector LaunchDirection, float LaunchSpeed);

    // 도착 지점을 계산하는 함수
    UFUNCTION(BlueprintPure, Category = "Projectile")
    FVector CalculateImpactPoint(float Gravity) const;
};
