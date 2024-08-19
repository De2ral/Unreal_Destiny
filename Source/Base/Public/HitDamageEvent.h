// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "HitDamageEvent.generated.h"

/**
 * 
 */
UCLASS()
class BASE_API UHitDamageEvent : public UDamageType
{
	GENERATED_BODY()
	
	public:
    UPROPERTY()
    FHitResult HitResult;

	UHitDamageEvent();

};
