// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "DestinyFPSBase.generated.h"

class UCharacterMovementComponent;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* SlideAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* SprintAction;



	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void jump(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void SprintEnd(const FInputActionValue& Value);
	void Slide(const FInputActionValue& Value);
	void SlideEnd(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bPlayerSprint = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Speed = 1.0f;

};
