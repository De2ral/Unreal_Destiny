// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Kallari.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CPP_Kallari.h"

AAIController_Kallari::AAIController_Kallari()
{
    PrimaryActorTick.bCanEverTick = true;

	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void AAIController_Kallari::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);
    
    if (UseBlackboard(BlackboardAsset, BlackboardComponent))
    {        
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"),false);	
    }
}

void AAIController_Kallari::BeginPlay()
{
    Super::BeginPlay();
    
    if(AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
	}
}

void AAIController_Kallari::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);	
    
	//GetBlackboardComponent()->SetValueAsVector(TEXT("TargetLocation"), PlayerPawn->GetActorLocation());		

	//if (GetPawn()->GetDistanceTo(PlayerPawn) <= 300)
	//{
	//	GetBlackboardComponent()->SetValueAsBool(TEXT("Attacking"), true);
	//}
	//else
	//	GetBlackboardComponent()->SetValueAsBool(TEXT("Attacking"), false);
}

void AAIController_Kallari::SetBlackboardValue(const FName &KeyName, bool Value)
{
    if (BlackboardComponent)
    {
        BlackboardComponent->SetValueAsBool(KeyName, Value);
    }
}

void AAIController_Kallari::HandleDeath()
{
    GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"),true);	
}
