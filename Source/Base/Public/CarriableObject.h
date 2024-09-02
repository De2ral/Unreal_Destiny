// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.h"
#include "CarriableObject.generated.h"

UCLASS()
class BASE_API ACarriableObject : public AInteractableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACarriableObject();

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ObjAction(ADestinyFPSBase* Player);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDestroy();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
