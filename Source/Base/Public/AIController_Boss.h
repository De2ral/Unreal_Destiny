// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController_Boss.generated.h"

/**
 * 
 */
UCLASS()
class BASE_API AAIController_Boss : public AAIController
{
	GENERATED_BODY()

public:	
	AAIController_Boss();

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
	
};
