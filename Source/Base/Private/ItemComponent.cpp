// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"

// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	AActor* Parent = GetOwner();
	
	ItemCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ItemCollider"));
	ItemCollider->InitSphereRadius(50.0f);
	ItemCollider->SetMobility(EComponentMobility::Movable);
	if(IsValid(Parent)) ItemCollider->SetupAttachment(Parent->GetRootComponent());	
	



	// ...
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	ItemCollider->OnComponentBeginOverlap.AddDynamic(this,&UItemComponent::OnOverlapBegin);
	AActor* Parent = GetOwner();

	if(IsValid(Parent)) ItemCollider->SetupAttachment(Parent->GetRootComponent());	
	
}

void UItemComponent::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

	AActor* Parent = GetOwner();

	if(OtherActor->ActorHasTag(TEXT("DestinyPlayer")) && Parent)
	{

		//여기에 아이템 - 인벤토리간 상호작용 코드



		Parent->Destroy();
	}

}

// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

