// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DestinyFPSBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USkeletalMeshComponent;

// class UCameraComponent;
// class UInputMappingContext;
// class UInputAction;

UCLASS()
class BASE_API ADestinyFPSBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADestinyFPSBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* TppSpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* FppCamera;

	UPROPERTY(EditAnywhere)
	UCameraComponent* TppCamera;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* FppMesh;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* TppMesh;

	void MoveForwardBackward(const float Value);
	void MoveLeftRight(const float Value);
	void LookUp(const float Value);
	void LookRight(const float Value);

	

};
