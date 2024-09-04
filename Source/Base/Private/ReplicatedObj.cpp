// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicatedObj.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "DestinyFPSBase.h"

// Sets default values
AReplicatedObj::AReplicatedObj()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ObjCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ObjCollider"));
	ObjMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjMesh"));
	AfterInteractMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AfterInteractMesh"));
	ObjCollider->SetupAttachment(RootComponent);
	ObjCollider->InitSphereRadius(130.0f);
	ObjMesh->SetupAttachment(ObjCollider);
	AfterInteractMesh->SetupAttachment(ObjMesh);

	AfterInteractMesh->SetVisibility(false);

}

void AReplicatedObj::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called when the game starts or when spawned
void AReplicatedObj::BeginPlay()
{
	Super::BeginPlay();

	if (QuestUIClass)
	{
		QuestUIInstance = CreateWidget<UUserWidget>(GetWorld(), QuestUIClass);
	}
}

void AReplicatedObj::ItemDrop(int32 ItemCount)
{
	for(int32 i = 0; i< ItemCount; ++i)
	{
		if (ItemsToSpawn.Num() > 0)
    	{
    	    int32 RandomIndex = FMath::RandRange(0, ItemsToSpawn.Num() - 1);
    	    TSubclassOf<AActor> SelectedItem = ItemsToSpawn[RandomIndex];

    	    if (SelectedItem)
    	    {
    	        FVector Location = GetActorLocation();
    	        FRotator Rotation = GetActorRotation();
    	        GetWorld()->SpawnActor<AActor>(SelectedItem, Location, Rotation);
    	    }
    	}
	}
}

// Called every frame
void AReplicatedObj::ShowQuestUI()
{
	QuestUIInstance->AddToViewport();
}

void AReplicatedObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
