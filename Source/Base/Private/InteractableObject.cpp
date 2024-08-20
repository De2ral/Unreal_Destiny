// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"
#include "DestinyFPSBase.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObjCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ObjCollider"));
	ObjMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjMesh"));
	ObjCollider->SetupAttachment(RootComponent);
	ObjCollider->InitSphereRadius(130.0f);
	ObjMesh->SetupAttachment(ObjCollider);

	

}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	ObjCollider->OnComponentBeginOverlap.AddDynamic(this,&AInteractableObject::OnOverlapBegin);
	ObjCollider->OnComponentEndOverlap.AddDynamic(this,&AInteractableObject::OnOverlapEnd);
}

void AInteractableObject::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(OtherActor->ActorHasTag(TEXT("DestinyPlayer")))
	{
		APlayer = Cast<ADestinyFPSBase>(OtherActor);
		//GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("Cast to Player - DeathOrb OverlapBegin"));
 		APlayer->bPlayerInteractable = true;
		APlayer->MaxInteractTime = ObjInteractTime;


	}	
	

}

void AInteractableObject::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->ActorHasTag(TEXT("DestinyPlayer")))
	{
		APlayer = Cast<ADestinyFPSBase>(OtherActor);
 		//GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("Cast to Player - DeathOrb OverlapEnd"));
 		APlayer->bPlayerInteractable = false;
	}
}


// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(APlayer && APlayer->bIsInteractComplete) 
	{
		ObjAction();
		APlayer->bIsInteractComplete = false;
	}
	

}

