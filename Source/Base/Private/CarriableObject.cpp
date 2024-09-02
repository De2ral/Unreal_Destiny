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

	if(!Player->GetIsPlayerCarrying()) Player->PlayerCarryingStart(this);
	
	if(HasAuthority())
    {
        Destroy();
    }
    else
    {
        ServerDestroy();  // 클라이언트에서는 서버에게 파괴 요청
    }

}

// Called every frame
void ACarriableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACarriableObject::ServerDestroy_Implementation()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Server is destroying the object"));
    if (IsValid(this))
    {
        GetWorld()->DestroyActor(this);
    }
}

bool ACarriableObject::ServerDestroy_Validate()
{
    return true;  // 이 함수가 검증을 통과했음을 의미
}

