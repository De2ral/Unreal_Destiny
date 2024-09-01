// Fill out your copyright notice in the Description page of Project Settings.


#include "CarryingInputObj.h"
#include "DestinyFPSBase.h"

// Sets default values
ACarryingInputObj::ACarryingInputObj()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AfterInteractMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AfterInteractMesh"));
	AfterInteractMesh->SetupAttachment(ObjMesh);
	ObjInteractTime = 70.0f;

}

// Called when the game starts or when spawned
void ACarryingInputObj::BeginPlay()
{
	Super::BeginPlay();
	AfterInteractMesh->SetVisibility(false);
	
}

void ACarryingInputObj::ObjAction(ADestinyFPSBase* Player)
{
	if(Player->GetIsPlayerCarrying())
	{
		Player->CarriedMeshComponent->DestroyComponent();
    	Player->CarriedMeshComponent = nullptr;
		Player->PlayerCarryingEnd();

		ObjMesh->SetStaticMesh(AfterInteractMesh->GetStaticMesh());
	}

}

// Called every frame
void ACarryingInputObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

