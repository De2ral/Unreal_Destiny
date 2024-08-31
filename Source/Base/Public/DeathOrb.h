// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.h"
#include "DeathOrb.generated.h"

class UPrimitiveComponent;

UCLASS()
class BASE_API ADeathOrb : public AInteractableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeathOrb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ObjAction(ADestinyFPSBase* Player);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
