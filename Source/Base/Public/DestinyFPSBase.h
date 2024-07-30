// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "DestinyFPSBase.generated.h"

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
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* TppSpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* FppCamera;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* TppCamera;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* FppMesh;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* TppMesh;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* LookAction;

protected:
	void Move(const FInputActionValue& Value);
};
