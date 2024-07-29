// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Dorongo.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CPP_Dorongo.h"

AAIController_Dorongo::AAIController_Dorongo()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAIController_Dorongo::BeginPlay()
{
    Super::BeginPlay();
    
    if(AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
	}
	// if (AIBehavior != nullptr)
	// {
	// 	RunBehaviorTree(AIBehavior);
	
	// 	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);	
        
	// 	GetBlackboardComponent()->SetValueAsVector(TEXT("TargetLocation"), PlayerPawn->GetActorLocation());				
	// 	GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());        
	// 	GetBlackboardComponent()->SetValueAsBool(TEXT("Attacking"), false); 		
	// 	GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"), false);
	
	// }
}

void AAIController_Dorongo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);	
    
	GetBlackboardComponent()->SetValueAsVector(TEXT("TargetLocation"), PlayerPawn->GetActorLocation());		
	// // if (LineOfSightTo(PlayerPawn))
	// // {
	// // }
	// // else
	// // {
	// // 	GetBlackboardComponent()->ClearValue(TEXT("TargetLocation"));
	// // }

	if (GetPawn()->GetDistanceTo(PlayerPawn) <= 1700)
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("Attacking"), true);
	}
	else
		GetBlackboardComponent()->SetValueAsBool(TEXT("Attacking"), false);
}

