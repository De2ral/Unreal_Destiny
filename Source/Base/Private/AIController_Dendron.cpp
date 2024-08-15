// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Dendron.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CPP_Dendron.h"

AAIController_Dendron::AAIController_Dendron()
{
	PrimaryActorTick.bCanEverTick = true;

	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void AAIController_Dendron::OnPossess(APawn *InPawn)
{
	Super::OnPossess(InPawn);

    // 블랙보드 초기화 코드가 필요하면 여기서 설정
    if (UseBlackboard(BlackboardAsset, BlackboardComponent))
    {
        // 초기 블랙보드 값 설정
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"),false);	
    }
}

void AAIController_Dendron::BeginPlay()
{
    Super::BeginPlay();
    
    if(AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
	}
}

void AAIController_Dendron::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);	
    
	GetBlackboardComponent()->SetValueAsVector(TEXT("TargetLocation"), PlayerPawn->GetActorLocation());		

	if (GetPawn()->GetDistanceTo(PlayerPawn) <= 3000)
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("Attacking"), true);
	}
	else
		GetBlackboardComponent()->SetValueAsBool(TEXT("Attacking"), false);

}

void AAIController_Dendron::SetBlackboardValue(const FName &KeyName, bool Value)
{
	if (BlackboardComponent)
    {
        BlackboardComponent->SetValueAsBool(KeyName, Value);
    }
}

void AAIController_Dendron::HandleDeath()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("BBBBBBBBBBBBBBBB -- HP ")));

	GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"),true);	

}
