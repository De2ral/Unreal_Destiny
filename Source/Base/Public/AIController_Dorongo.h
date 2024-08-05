// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController_Dorongo.generated.h"

/**
 * 
 */
UCLASS()
class BASE_API AAIController_Dorongo : public AAIController
{
	GENERATED_BODY()
	
public:	
	AAIController_Dorongo();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

};
