// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "WeaponComponent.h"
#include "SkillWidget.h"
#include "HUDWidget.h"

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
	virtual void Tick(float DeltaTime) override;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* SlideAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* InterAction;

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsSliding;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bPlayerSprint = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SlideTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InteractTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxInteractTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPlayerInteractable = false;


	// Skill Variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isShield = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isGrenade = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillCoolTime = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GrenadeCoolTime = 3.f;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UHUDWidget> HUDWidgetClass;

    UPROPERTY()
    UHUDWidget* HUDWidget;

	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Skill(const FInputActionValue& Value);
	void Grenade(const FInputActionValue& Value);
	void jump(const FInputActionValue& Value);
	void Ultimate(const FInputActionValue& Value);

	void Sprint(const FInputActionValue& Value);
	void SprintEnd(const FInputActionValue& Value);

	void Slide(const FInputActionValue& Value);
	void SlideEnd(const FInputActionValue& Value);

	void StartInteract(const FInputActionValue& Value);
	void EndInteract(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Weapon")
    UWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);
	
	USkeletalMeshComponent* GetFppMesh() const { return FppMesh; }
	void Throw();
	
private:
	float CurSkillCoolTime = SkillCoolTime;
	float CurGrenadeCoolTime = GrenadeCoolTime;
};
