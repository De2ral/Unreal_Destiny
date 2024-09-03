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
	ObjCollider->SetupAttachment(RootComponent);
	ObjCollider->InitSphereRadius(130.0f);
	ObjMesh->SetupAttachment(ObjCollider);

}

// Called when the game starts or when spawned
void AReplicatedObj::BeginPlay()
{
	Super::BeginPlay();
	ObjCollider->OnComponentBeginOverlap.AddDynamic(this,&AReplicatedObj::OnOverlapBegin);
	ObjCollider->OnComponentEndOverlap.AddDynamic(this,&AReplicatedObj::OnOverlapEnd);
}

// void AReplicatedObj::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
// {
//     Super::GetLifetimeReplicatedProps(OutLifetimeProps);

// 	DOREPLIFETIME(AReplicatedObj, MinItemValue);
//     DOREPLIFETIME(AReplicatedObj, MaxItemValue);
//     DOREPLIFETIME(AReplicatedObj, ItemsToSpawn);

// }

void AReplicatedObj::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
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

void AReplicatedObj::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
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

void AReplicatedObj::ServerRepliObjAction_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("서버에서 계산해"));
	RepliObjAction(APlayer);

}

bool AReplicatedObj::ServerRepliObjAction_Validate()
{
	return true;
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
void AReplicatedObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(APlayer && APlayer->bIsInteractComplete) 
	{
		RepliObjAction(APlayer);
	}

}

void AReplicatedObj::RepliObjAction(ADestinyFPSBase *Player)
{

	if(HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("InteractableObj -> MyStash.RepliObjAction()"));

    	int32 ItemCount = FMath::RandRange(MinItemValue, MaxItemValue);	
    	ItemDrop(ItemCount);

    	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Authority: Server, Destroying on server"));
		APlayer->bIsInteractComplete = false;
    	Destroy();
    	
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("클라면 서버로 가"));
		ServerRepliObjAction();
	} 
	
}
