// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DorongoAttack.generated.h"

/**
 * 
 */
UCLASS()
class BASE_API UBTTask_DorongoAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_DorongoAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
