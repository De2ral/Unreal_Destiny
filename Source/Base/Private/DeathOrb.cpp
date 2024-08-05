// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathOrb.h"
#include "DestinyFPSBase.h"

// Sets default values
ADeathOrb::ADeathOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Orb = CreateDefaultSubobject<USphereComponent>(TEXT("DeathOrbCollider"));
	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrbMesh"));
	Orb->SetupAttachment(RootComponent);
	Orb->InitSphereRadius(130.0f);
	OrbMesh->SetupAttachment(Orb);


}

// Called when the game starts or when spawned
void ADeathOrb::BeginPlay()
{
	Super::BeginPlay();
	Orb->OnComponentBeginOverlap.AddDynamic(this,&ADeathOrb::OnOverlapBegin);
	Orb->OnComponentEndOverlap.AddDynamic(this,&ADeathOrb::OnOverlapEnd);
	
}

void ADeathOrb::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(OtherActor->ActorHasTag(TEXT("DestinyPlayer")))
	{
		ADestinyFPSBase* APlayer = Cast<ADestinyFPSBase>(OtherActor);
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("Cast to Player - DeathOrb OverlapBegin"));
 		APlayer->bPlayerInteractable = true;
		APlayer->MaxInteractTime = 180.0f;

	}

}

void ADeathOrb::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{

	if(OtherActor->ActorHasTag(TEXT("DestinyPlayer")))
	{
		ADestinyFPSBase* APlayer = Cast<ADestinyFPSBase>(OtherActor);
 		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("Cast to Player - DeathOrb OverlapEnd"));
 		APlayer->bPlayerInteractable = false;


	}

}

// Called every frame
void ADeathOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

