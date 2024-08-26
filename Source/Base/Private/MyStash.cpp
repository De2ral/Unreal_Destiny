// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStash.h"

#include "DestinyFPSBase.h"

// Sets default values
AMyStash::AMyStash()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//StashMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StashMesh"));

	//Collider = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionTest"));
	//Collider->SetupAttachment(RootComponent);

	//StashMesh->SetupAttachment(Collider);

	ObjInteractTime = 40.0f;

}

// Called when the game starts or when spawned
void AMyStash::BeginPlay()
{
	Super::BeginPlay();
	//Collider->OnComponentBeginOverlap.AddDynamic(this,&AMyStash::OnOverlapBegin);
	//Collider->OnComponentEndOverlap.AddDynamic(this,&AMyStash::OnOverlapEnd);
	
}

void AMyStash::ObjAction()
{
	GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Cyan,TEXT("InteractableObj -> MyStash.ObjAction()"));

}

//void AMyStash::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
//{
//	if(OtherActor->ActorHasTag(TEXT("DestinyPlayer")))
//	{
//		ADestinyFPSBase* APlayer = Cast<ADestinyFPSBase>(OtherActor);
//		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("Cast to Player - OverlapBegin"));
// 		APlayer->bPlayerInteractable = true;
//		APlayer->MaxInteractTime = 50.0f;
//
//	}
//
//}
//
//void AMyStash::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
//{
//
//	if(OtherActor->ActorHasTag(TEXT("DestinyPlayer")))
//	{
//		ADestinyFPSBase* APlayer = Cast<ADestinyFPSBase>(OtherActor);
// 		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("Cast to Player - OverlapEnd"));
// 		APlayer->bPlayerInteractable = false;
//
//	}
//
//}

// Called every frame
void AMyStash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

