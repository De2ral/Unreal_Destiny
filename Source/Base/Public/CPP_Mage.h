// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_MonsterBase.h"
#include "GameFramework/Character.h"
#include "CPP_Mage.generated.h"

/**
 * 
 */
UCLASS()
class BASE_API ACPP_Mage : public ACPP_MonsterBase
{
	GENERATED_BODY()
	
public:
	ACPP_Mage();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void PerformLineTrace();
	FVector StartPoint;
	FVector EndPoint;

};
