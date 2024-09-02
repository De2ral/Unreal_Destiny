// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStash.h"
#include "Net/UnrealNetwork.h"
#include "DestinyFPSBase.h"

// Sets default values
AMyStash::AMyStash()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Replicate this actor
    bReplicates = true;

	ObjInteractTime = 40.0f;

}

// Called when the game starts or when spawned
void AMyStash::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyStash::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate any relevant variables
    DOREPLIFETIME(AMyStash, MinItemValue);
    DOREPLIFETIME(AMyStash, MaxItemValue);
    DOREPLIFETIME(AMyStash, ItemsToSpawn);
}

void AMyStash::ObjAction(ADestinyFPSBase* Player)
{

 	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("InteractableObj -> MyStash.ObjAction()"));

    int32 ItemCount = FMath::RandRange(MinItemValue, MaxItemValue);	
    ItemDrop(ItemCount);

    if (HasAuthority()) // 서버일 경우
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Authority: Server, Destroying on server"));
        Destroy();
    }
    else // 클라이언트일 경우 서버에 파괴 요청
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Authority: Client, Requesting server to destroy"));
        ServerDestroy();
    }

}


void AMyStash::ItemDrop(int32 ItemCount)
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

void AMyStash::ServerDestroy_Implementation()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Server is destroying the object"));
    Destroy();
}


bool AMyStash::ServerDestroy_Validate()
{
    return true;  // 이 함수가 검증을 통과했음을 의미
}

// Called every frame
void AMyStash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

