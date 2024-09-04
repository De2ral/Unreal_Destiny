// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_DeathOrb.h"
#include "DestinyFPSBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACPP_DeathOrb::ACPP_DeathOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OrbCollider = CreateDefaultSubobject<USphereComponent>(TEXT("OrbCollider"));
	OrbCollider->InitSphereRadius(70.0f);

	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrbMesh"));
	OrbMesh->SetWorldScale3D(FVector3d(3.0f,3.0f,3.0f));

	OrbMesh->SetupAttachment(RootComponent);
	OrbCollider->SetupAttachment(OrbMesh);

	bReplicates = true;

}

// Called when the game starts or when spawned
void ACPP_DeathOrb::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACPP_DeathOrb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACPP_DeathOrb, DeadPlayer);  // DeadPlayer를 리플리케이션
}

void ACPP_DeathOrb::ServerOnInteract_Implementation(ADestinyFPSBase* InteractingPlayer)
{
    if (DeadPlayer)
    {
        // 죽은 플레이어 소생 처리
        DeadPlayer->Revive();

        Destroy();
    }
}

bool ACPP_DeathOrb::ServerOnInteract_Validate(ADestinyFPSBase* InteractingPlayer)
{
    return true;  // 간단한 검증, 필요시 추가 검증 로직 구현
}

// Called every frame
void ACPP_DeathOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

