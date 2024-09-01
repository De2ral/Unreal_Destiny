// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController_Mage.generated.h"

/**
 * 
 */

UCLASS()
class BASE_API AAIController_Mage : public AAIController
{
	GENERATED_BODY()

public:	
	AAIController_Mage();

	virtual void OnPossess(APawn* InPawn) override;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "AI")
    void SetBlackboardValue(const FName& KeyName, bool Value);

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	UPROPERTY(Transient)
    UBlackboardComponent* BlackboardComponent;

    UPROPERTY(Transient)
    UBehaviorTreeComponent* BehaviorTreeComponent;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBlackboardData* BlackboardAsset;

public:
	void HandleDeath();

	bool CanLookTarget = false;


public:
    AActor* GetRandomPlayerPawn();

	UPROPERTY(BlueprintReadOnly, Category = "Target")
    APawn* CurrentRandomPlayerPawn;

    UFUNCTION(BlueprintCallable, Category = "Target")
    void UpdateRandomPlayerPawn();
	
public:	
    TArray<APawn*> GetAllPlayerPawns();
	void SelectRandomPlayerPawn();
	

UPROPERTY(BlueprintReadOnly, Category = "Target")
    APawn* ClosestPlayerPawn;

    // 블루프린트에서 호출할 수 있는 함수
    UFUNCTION(BlueprintCallable, Category = "Target")
    void FindClosestPlayer();

private:
    APawn* GetClosestPlayerPawn();
	
};
