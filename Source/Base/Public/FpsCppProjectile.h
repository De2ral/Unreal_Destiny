// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FpsCppProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS(config=Game)
class AFpsCppProjectile : public AActor
{
	GENERATED_BODY()

public:
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;

public:
	AFpsCppProjectile();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

	void FireInDirection(const FVector& ShootDirection);

	void SetProjectile(UStaticMesh* NewMesh, float speed, float damage);

	void SetProjectileMesh(UStaticMesh* NewMesh);

	void AttachTrailEffect(bool isRifle);

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	void SetbExplodeOnImpact(bool inbool) {bExplodeOnImpact = inbool;}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void DelayedDestroy();
private:
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ExplosionRadius = 1000.0f;

	UPROPERTY(Replicated, EditAnywhere, Category = "Combat")
	bool bExplodeOnImpact = false;

	UParticleSystem* HitFlash;
	UParticleSystem* HitFlash_Launcher;

	FTimerHandle TrailTimerHandle;
	UNiagaraSystem* TrailEffect;

	UNiagaraSystem* TrailEffect_Launcher;
};

