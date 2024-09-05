// Fill out your copyright notice in the Description page of Project Settings.

#include "AIController_Murdock.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "CPP_Murdock.h"

AAIController_Murdock::AAIController_Murdock()
{
	PrimaryActorTick.bCanEverTick = true;

	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void AAIController_Murdock::OnPossess(APawn *InPawn)
{
	Super::OnPossess(InPawn);

    // 블랙보드 초기화 코드가 필요하면 여기서 설정
    if (UseBlackboard(BlackboardAsset, BlackboardComponent))
    {
        // 초기 블랙보드 값 설정
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"),false);	
    }
}

void AAIController_Murdock::BeginPlay()
{
    Super::BeginPlay();
    //UpdateRandomPlayerPawn();
	//FindClosestPlayer();
    if(AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
	}
}

void AAIController_Murdock::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AAIController_Murdock::SetBlackboardValue(const FName &KeyName, bool Value)
{
	if (BlackboardComponent)
    {
        BlackboardComponent->SetValueAsBool(KeyName, Value);
    }
}

void AAIController_Murdock::HandleDeath()
{
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("BBBBBBBBBBBBBBBB -- HP ")));

	GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"),true);	

}

void AAIController_Murdock::UpdateRandomPlayerPawn()
{
	//SelectRandomPlayerPawn();
}

void AAIController_Murdock::SelectRandomPlayerPawn()
{
	 TArray<APawn*> PlayerPawns = GetAllPlayerPawns();

    if (PlayerPawns.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, PlayerPawns.Num() - 1);
        CurrentRandomPlayerPawn = PlayerPawns[RandomIndex];
    }
    else
    {
        CurrentRandomPlayerPawn = nullptr;
    }
}

void AAIController_Murdock::FindClosestPlayer()
{
	 ClosestPlayerPawn = GetClosestPlayerPawn();
}

APawn *AAIController_Murdock::GetClosestPlayerPawn()
{
    APawn* ClosestPawn = nullptr;
    float ClosestDistanceSquared = FLT_MAX; 

    UWorld* World = GetWorld();
    if (World)
    {
        APawn* MyPawn = GetPawn();

        for (TActorIterator<APlayerController> It(World); It; ++It)
        {
            APlayerController* PlayerController = *It;
            if (PlayerController)
            {
                APawn* PlayerPawn = PlayerController->GetPawn();
                if (PlayerPawn && PlayerPawn != MyPawn) 
                {
                    float DistanceSquared = (PlayerPawn->GetActorLocation() - MyPawn->GetActorLocation()).SizeSquared();
                    if (DistanceSquared < ClosestDistanceSquared)
                    {
                        ClosestDistanceSquared = DistanceSquared;
                        ClosestPawn = PlayerPawn;
                    }
                }
            }
        }
    }

    return ClosestPawn;
}

TArray<APawn*> AAIController_Murdock::GetAllPlayerPawns()
{
    TArray<APawn*> PlayerPawns;

    UWorld* World = GetWorld();
    if (World)
    {
        for (TActorIterator<APlayerController> It(World); It; ++It)
        {
            APlayerController* PlayerController = *It;
            if (PlayerController)
            {
                APawn* PlayerPawn = PlayerController->GetPawn();
                if (PlayerPawn)
                {
                    PlayerPawns.Add(PlayerPawn);
                }
            }
        }
    }

    return PlayerPawns;
}
