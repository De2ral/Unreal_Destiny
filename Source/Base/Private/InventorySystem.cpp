// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

// Sets default values for this component's properties
UInventorySystem::UInventorySystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventorySystem::BeginPlay()
{
	//Super::BeginPlay();

	// ...
	
}

void UInventorySystem::InvenOpenClose()
{
	if(bIsInvenOpen)
	{
		GEngine->AddOnScreenDebugMessage(-1,0.5f,FColor::Red,TEXT("인벤토리가 열렸다"));

	}

	else if(!bIsInvenOpen)
	{
		GEngine->AddOnScreenDebugMessage(-1,0.5f,FColor::Red,TEXT("인벤토리가 닫혔다"));
		
	}


}

// Called every frame
void UInventorySystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventorySystem::AddMapping(ADestinyFPSBase* TargetPlayer)
{
	
	if(APlayerController* PlayerController = Cast<APlayerController>(TargetPlayer->GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(InventoryMappingContext,1);

		}

		if(UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			Input->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &UInventorySystem::InvenOpenClose);

		}
		

	}


}

