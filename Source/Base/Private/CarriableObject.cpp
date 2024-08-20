// Fill out your copyright notice in the Description page of Project Settings.


#include "CarriableObject.h"
#include "DestinyFPSBase.h"

// Sets default values
ACarriableObject::ACarriableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	//CarriableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarriableMesh"));
	//Collider->SetupAttachment(RootComponent);
	//Collider->InitSphereRadius(80.0f);
	//CarriableMesh->SetupAttachment(Collider);

	ObjInteractTime = 60.0f;

}

// Called when the game starts or when spawned
void ACarriableObject::BeginPlay()
{
	Super::BeginPlay();

	//Collider->OnComponentBeginOverlap.AddDynamic(this,&ACarriableObject::OnOverlapBegin);
	//Collider->OnComponentEndOverlap.AddDynamic(this,&ACarriableObject::OnOverlapEnd);
	
}

//void ACarriableObject::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
//{
//	if(OtherActor->ActorHasTag(TEXT("DestinyPlayer")))
//	{
//		ADestinyFPSBase* APlayer = Cast<ADestinyFPSBase>(OtherActor);
//		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("Cast to Player - Carriable OverlapBegin"));
// 		APlayer->bPlayerInteractable = true;
//		APlayer->MaxInteractTime = 40.0f;
//
//	}
//}
//
//void ACarriableObject::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
//{
//	if(OtherActor->ActorHasTag(TEXT("DestinyPlayer")))
//	{
//		ADestinyFPSBase* APlayer = Cast<ADestinyFPSBase>(OtherActor);
//		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("Cast to Player - Carriable OverlapEnd"));
// 		APlayer->bPlayerInteractable = false;
//
//	}
//}

void ACarriableObject::ObjAction()
{
	GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Cyan,TEXT("InteractableObj -> Carriable.ObjAction()"));

}

// Called every frame
void ACarriableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

