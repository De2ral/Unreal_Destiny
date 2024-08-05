// Fill out your copyright notice in the Description page of Project Settings.


#include "Titan_Skill_Barrier.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
ATitan_Skill_Barrier::ATitan_Skill_Barrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->SetRelativeScale3D(FVector(0.1f, 4.f, 1.5f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
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

}

void ATitan_Skill_Barrier::DestroyBarrier()
{
	Destroy();
}

