// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "DestinyFPSBase.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

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
    if (OtherActor->ActorHasTag("DestinyPlayer"))
    {
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Yellow,TEXT("isPlayer"));
        APlayer = Cast<ADestinyFPSBase>(OtherActor);

        if (APlayer->GetIsPlayerAlive())
        {
            APlayer->bPlayerInteractable = true;
            APlayer->MaxInteractTime = ObjInteractTime;
        }
        
    }
}

void AInteractableObject::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	
	if (OtherActor->ActorHasTag("DestinyPlayer"))
    {
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Yellow,TEXT("isPlayer"));
        APlayer = Cast<ADestinyFPSBase>(OtherActor);
	
	    if (APlayer->GetIsPlayerAlive())
	    {
	        APlayer->bPlayerInteractable = false;
	    }
	
	}
	
}

void AInteractableObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(APlayer && APlayer->bIsInteractComplete) 
	{
		ServerObjAction_Implementation();
		APlayer->bIsInteractComplete = false;
	}
	

}

void AInteractableObject::ServerObjAction_Implementation()
{
	if (HasAuthority()) // 서버에서 호출
    {
        MultiCastObjAction();
    }
    else // 클라이언트에서 호출
    {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Client to Server"));
        ObjAction(APlayer);
    }

}

void AInteractableObject::MultiCastObjAction_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Server to Everyone"));
	ObjAction(APlayer);

}

