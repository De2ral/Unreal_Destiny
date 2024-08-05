// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"

// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	AActor* Owner = GetOwner();

	ItemCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ItemCollider"));
	ItemCollider->InitSphereRadius(50.0f);


	// ...
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UItemComponent::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

	// if(OtherActor->ActorHasTag(TEXT("DestinyPlayer") && IsValid(Owner)))
	// {
	// 	Destory(Owner);
	// }

}

// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

