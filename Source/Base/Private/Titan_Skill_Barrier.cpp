// Fill out your copyright notice in the Description page of Project Settings.


#include "Titan_Skill_Barrier.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

// Sets default values
ATitan_Skill_Barrier::ATitan_Skill_Barrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	Mesh->OnComponentHit.AddDynamic(this, &ATitan_Skill_Barrier::OnHit);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Skill/Titan/Mesh_Shield.Mesh_Shield'"));
	if(MeshAsset.Succeeded())
		Mesh->SetStaticMesh(MeshAsset.Object);
}

// Called when the game starts or when spawned
void ATitan_Skill_Barrier::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATitan_Skill_Barrier::DestroyBarrier, DestroyDelay, false);
}

// Called every frame
void ATitan_Skill_Barrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MoveTime > 0.f)
	{
		FVector P0 = GetActorLocation();
		FVector vt = -GetActorRightVector() * MoveSpeed * DeltaTime;
		FVector P = P0 + vt;
		SetActorLocation(P);
		MoveTime -= DeltaTime;
	}
}

void ATitan_Skill_Barrier::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	OtherActor->Destroy();
}

void ATitan_Skill_Barrier::DestroyBarrier()
{
	Destroy();
}