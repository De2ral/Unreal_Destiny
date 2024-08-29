// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.h"
#include "CarryingInputObj.generated.h"

UCLASS()
class BASE_API ACarryingInputObj : public AInteractableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACarryingInputObj();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ObjAction(ADestinyFPSBase* Player);

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* AfterInteractMesh;

	bool bIsFill = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool GetObjIsFill() {return bIsFill; }

};
