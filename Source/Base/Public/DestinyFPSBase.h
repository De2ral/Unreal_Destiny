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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* SkillAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* GrenadeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* UltimateAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Skill(const FInputActionValue& Value);
	void Grenade(const FInputActionValue& Value);
	void Ultimate(const FInputActionValue& Value);
};
