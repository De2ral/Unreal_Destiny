// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CPP_DeathOrb.generated.h"

UCLASS()
class BASE_API ACPP_DeathOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_DeathOrb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* OrbCollider;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* OrbMesh;

    UPROPERTY(Replicated)
    class ADestinyFPSBase* DeadPlayer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float ObjInteractTime = 150.0f;

	 // 죽은 플레이어 설정 함수
    void SetDeadPlayer(ADestinyFPSBase* Player) { DeadPlayer = Player; }

	ADestinyFPSBase* GetDeadPlayer() { return DeadPlayer; }

    // 다른 플레이어가 상호작용할 때 호출될 함수 (서버에서 실행)
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerOnInteract(ADestinyFPSBase* InteractingPlayer);

};
