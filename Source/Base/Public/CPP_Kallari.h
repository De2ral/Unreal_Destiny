// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_MonsterBase.h"
#include "GameFramework/Character.h"
#include "CPP_Kallari.generated.h"

/**
 * 
 */
UCLASS()
class BASE_API ACPP_Kallari : public ACPP_MonsterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_Kallari();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
