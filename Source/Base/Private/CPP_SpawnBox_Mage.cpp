// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_SpawnBox_Mage.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "CPP_Mage.h"
#include "AIController_Mage.h"

// Sets default values
ACPP_SpawnBox_Mage::ACPP_SpawnBox_Mage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	RootComponent = SpawnBox;

	TargetMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMeshComponent"));
    TargetMeshComponent->SetupAttachment(RootComponent);

	SpawnInterval = 5.0f; // 스폰 시간

}

// Called when the game starts or when spawned
void ACPP_SpawnBox_Mage::BeginPlay()
{
	Super::BeginPlay();
	CreateTargetMesh();
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle,this,&ACPP_SpawnBox_Mage::OnSpawnTimerElapsed,SpawnInterval,true);
}

// Called every frame
void ACPP_SpawnBox_Mage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ACPP_SpawnBox_Mage::GetTargetLocation_() const
{
    if (TargetMeshComponent)
    {
        return TargetMeshComponent->GetComponentLocation();
    }
    return FVector::ZeroVector;
}

void ACPP_SpawnBox_Mage::SpawnEnemy()
{
	if(HasAuthority())
	{
	 if (EnemyClass)
     {
        FVector SpawnLocation = FMath::RandPointInBox(SpawnBox->Bounds.GetBox());
        FRotator SpawnRotation = FRotator::ZeroRotator;

        //GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotation);
		
		FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

		SpawnedEnemy = GetWorld()->SpawnActor<ACPP_Mage>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);

		if (SpawnedEnemy)
        {
			if(AIControllerClass)
			{
            	AAIController_Mage* AIController = GetWorld()->SpawnActor<AAIController_Mage>(AIControllerClass);
            	if (AIController)
            	{
					FVector TargetLocation = TargetMeshComponent->GetComponentLocation();
					AIController->MoveToLocation(TargetLocation);
               	 	AIController->Possess(SpawnedEnemy);
            	}
			}
        }

		if(GEngine)
		{
			FString Message = FString::Printf(TEXT("Spawned Enemy at location: %s"), *SpawnLocation.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);
		}

    }
	}
}

void ACPP_SpawnBox_Mage::OnSpawnTimerElapsed()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Check"));

	if (IsEnemyValid()) return;

    SpawnEnemy();
}

bool ACPP_SpawnBox_Mage::IsEnemyValid()
{    
    return SpawnedEnemy && !SpawnedEnemy->IsPendingKill();
}

void ACPP_SpawnBox_Mage::CreateTargetMesh()
{
	if (TargetMeshComponent)
    {
        TargetMeshComponent->SetRelativeLocation(FVector(0, 0, 0)); // 위치 조정
    }
}
