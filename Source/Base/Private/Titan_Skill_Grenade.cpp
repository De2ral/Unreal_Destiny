// Fill out your copyright notice in the Description page of Project Settings.


#include "Titan_Skill_Grenade.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATitan_Skill_Grenade::ATitan_Skill_Grenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/ParagonDrongo/FX/Meshes/Heroes/Drongo/SM_Drongo_Grenade_FX_Body02.SM_Drongo_Grenade_FX_Body02'"));
	if (MeshAsset.Succeeded())
		Mesh->SetStaticMesh(MeshAsset.Object);
}

// Called when the game starts or when spawned
void ATitan_Skill_Grenade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATitan_Skill_Grenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

