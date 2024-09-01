// Fill out your copyright notice in the Description page of Project Settings.

#include "DeathOrb.h"
#include "DestinyFPSBase.h"

// Sets default values
ADeathOrb::ADeathOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObjInteractTime = 150.0f;

}

// Called when the game starts or when spawned
void ADeathOrb::BeginPlay()
{
	Super::BeginPlay();
	//Obj->OnComponentBeginOverlap.AddDynamic(this,&ADeathOrb::OnOverlapBegin);
	//Obj->OnComponentEndOverlap.AddDynamic(this,&ADeathOrb::OnOverlapEnd);
	
}

void ADeathOrb::ObjAction(ADestinyFPSBase* Player)
{
	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,TEXT("InteractableObj -> DeathOrb.ObjAction"));
	Destroy();

}

// Called every frame
void ADeathOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

