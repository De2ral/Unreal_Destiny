// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DorongoAttack.h"
#include "AIController.h"
#include "CPP_Dorongo.h"

UBTTask_DorongoAttack::UBTTask_DorongoAttack()
{
    NodeName = "Attack";
}

EBTNodeResult::Type UBTTask_DorongoAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

    ACPP_Dorongo* dorongo = Cast<ACPP_Dorongo>(OwnerComp.GetAIOwner()->GetPawn());

    if(dorongo == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    return EBTNodeResult::Type();
}
