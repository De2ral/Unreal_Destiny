// Fill out your copyright notice in the Description page of Project Settings.


#include "CarriableObject.h"
#include "DestinyFPSBase.h"

// Sets default values
ACarriableObject::ACarriableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ObjInteractTime = 60.0f;

}

// Called when the game starts or when spawned
void ACarriableObject::BeginPlay()
{
	Super::BeginPlay();

	
}

void ACarriableObject::ObjAction(ADestinyFPSBase* Player)
{
	GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Cyan,TEXT("InteractableObj -> Carriable.ObjAction()"));

	Player->SwitchToThirdPerson();

	if(!Player->GetIsPlayerCarrying()) Player->PlayerCarryingStart(this);
	else if(Player->GetIsPlayerCarrying()) Player->PlayerCarryingEnd();
	
	Destroy();

}

// Called every frame
void ACarriableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

