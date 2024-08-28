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

UENUM(BlueprintType)
enum class EPlayerClassEnum : uint8
{
	HUNTER UMETA(DisplayName = "Hunter"),
	TITAN UMETA(DisplayName = "Titan"),
	WARLOCK UMETA(DisplayName = "Warlock")
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* FppMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
	class UInputAction* InventoryAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* LeftClickAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* RightClickAction;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerClassEnum PlayerClass;

	// Skill Variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isSkill = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isGrenade = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isUltimate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isSmash = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isMeleeAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillCoolTime = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GrenadeCoolTime = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UltimateCoolTime = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UltimateDuration = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SmashCoolTime = 7.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeleeAttackCoolTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UltimateDamage = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TitanPunchDamage = 75.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	class USphereComponent* TitanSmashCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	class USphereComponent* TitanPunchCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	class USphereComponent* TitanPunchDamageCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* TitanPunchStartParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* TitanPunchParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* TitanUltimateSmashParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* TitanUltimateFistParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* TitanUltimateLaunchParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* TitanUltimateTrailParticle;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UHUDWidget> HUDWidgetClass;

 	UPROPERTY()
    UHUDWidget* HUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<class AGrenade> GrenadeClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	bool bIsInvenOpen = false;
	
	void InvenOpenClose();
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

	void LeftClickFunction(const FInputActionValue& Value);
	void RightClickFunction(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Weapon")
    UWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);
	
	USkeletalMeshComponent* GetFppMesh() const { return FppMesh; }

	void Shield();
	void Throw();
	void EndUltimate();

	UFUNCTION(BlueprintCallable)
	void CameraShake(float Scale);

	UFUNCTION(BlueprintCallable)
	void TitanMeleeAttackStart(float ZDirection, float LaunchStrength);

	UFUNCTION(BlueprintCallable)
	void TitanMeleeAttackEnd();

	UFUNCTION(BlueprintCallable)
	void TitanSmashStart(float ZDirection, float LaunchStrength, float GravityScale, FVector CameraLocation);

	UFUNCTION(BlueprintCallable)
	void TitanSmashTop(float ZDirection, float LaunchStrength, float GravityScale);

	UFUNCTION(BlueprintCallable)
	void TitanSmashDown();

	UFUNCTION(BlueprintCallable)
	void TitanSmashEnd(float DelayTime);

	UFUNCTION(BlueprintCallable)
	void SwitchToFirstPerson();

	UFUNCTION(BlueprintCallable)
	void SwitchToThirdPerson();

	void SetClassValue();
	void TitanPunchCollisionEvents();
	
private:
	float CurSkillCoolTime = SkillCoolTime;
	float CurGrenadeCoolTime = GrenadeCoolTime;
	float CurUltimateCoolTime = UltimateCoolTime;
	float CurUltimateDuration = UltimateDuration;
	float CurSmashCoolTime = SmashCoolTime;
	float CurMeleeAttackCoolTime = MeleeAttackCoolTime;

	bool isTitanPunch = false;

	USkeletalMesh* SelectedMesh = nullptr;
	TSubclassOf<UAnimInstance> SelectedAnimInstanceClass = nullptr;
};
