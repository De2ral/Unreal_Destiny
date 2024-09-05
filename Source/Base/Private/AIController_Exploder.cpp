// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Exploder.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "CPP_Exploder.h"


AAIController_Exploder::AAIController_Exploder()
{
    PrimaryActorTick.bCanEverTick = true;

	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void AAIController_Exploder::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);
    
    if (UseBlackboard(BlackboardAsset, BlackboardComponent))
    {        
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"),false);	
    }
}

void AAIController_Exploder::BeginPlay()
{
    Super::BeginPlay();
    
    if(AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
	}
}

void AAIController_Exploder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);	
    
	GetBlackboardComponent()->SetValueAsVector(TEXT("TargetLocation"), PlayerPawn->GetActorLocation());		

	if (GetPawn()->GetDistanceTo(PlayerPawn) <= 300)
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("Attacking"), true);
	}
	else
		GetBlackboardComponent()->SetValueAsBool(TEXT("Attacking"), false);
}

void AAIController_Exploder::SetBlackboardValue(const FName &KeyName, bool Value)
{
    if (BlackboardComponent)
    {
        BlackboardComponent->SetValueAsBool(KeyName, Value);
    }
}

void AAIController_Exploder::HandleDeath()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("BBBBBBBBBBBBBBBB -- HP ")));

    GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"),true);	
}



void AAIController_Exploder::UpdateRandomPlayerPawn()
{
	//SelectRandomPlayerPawn();
}

void AAIController_Exploder::SelectRandomPlayerPawn()
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

void AAIController_Exploder::FindClosestPlayer()
{
	 ClosestPlayerPawn = GetClosestPlayerPawn();
}

APawn *AAIController_Exploder::GetClosestPlayerPawn()
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

TArray<APawn*> AAIController_Exploder::GetAllPlayerPawns()
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
