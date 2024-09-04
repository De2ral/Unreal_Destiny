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
class ACarriableObject;

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

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* SpearMesh;

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
	class UInputAction* MeleeAttackAction;

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
	class UInputAction* DeathReviveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsSliding = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bPlayerSprint = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SlideTime = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InteractTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxInteractTime = 0.0f;

	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadOnly)
	float HP = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHp = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	bool bIsInteractComplete = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPlayerInteractable = false;

	UPROPERTY(EditDefaultsOnly)
	FVector SlideVector;

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
	float UltimateCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UltimateDuration = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SmashCoolTime = 7.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SmashRadius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeleeAttackCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TitanUltimateDamage = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TitanPunchDamage = 75.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TitanPunchRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpawnWarlockUltimateDistance = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WarlockSkillDamage = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WarlockSkillHealAmount = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WarlockSkillRadius = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HunterUltimateAttackDamage = 40.f;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* HunterComboMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	class USphereComponent* TitanPunchCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	class USphereComponent* WarlockSkillCollider;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* WarlockSkillStartParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* WarlockSkillLandParticle;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UHUDWidget> HUDWidgetClass;

 	UPROPERTY()
    UHUDWidget* HUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<class AGrenade> GrenadeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<class AWarlock_Skill_Ultimate> WarlockUltimateClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<class AWarlock_Melee_Fireball> WarlockFireballClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	bool bIsInvenOpen = false;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	bool bPlayerIsMoving = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> DeathOrbTest;

	AActor* SpawnedDeathOrb;
	
	void InvenOpenClose();

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
	void Skill(const FInputActionValue& Value);
	void Grenade(const FInputActionValue& Value);
	void MeleeAttack(const FInputActionValue& Value);

	void jump(const FInputActionValue& Value);
	void jumpEnd(const FInputActionValue& Value);
	void Ultimate(const FInputActionValue& Value);

	void Sprint(const FInputActionValue& Value);
	void SprintEnd(const FInputActionValue& Value);

	void Slide(const FInputActionValue& Value);
	void SlideEnd(const FInputActionValue& Value);

	void StartInteract(const FInputActionValue& Value);
	void EndInteract(const FInputActionValue& Value);

	void LeftClickFunction(const FInputActionValue& Value);
	void RightClickFunction(const FInputActionValue& Value);
	void HPDamageTest(const FInputActionValue& Value);

	void Death();
	void Revive();

	UPROPERTY(EditAnywhere, Category = "Weapon")
    UWeaponComponent* WeaponComponent;

	UPROPERTY(EditAnyWhere)
	UStaticMeshComponent* CarriedMeshComponent;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool bHasRifle;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);
	
	USkeletalMeshComponent* GetFppMesh() const { return FppMesh; }

	void Throw();
	void EndUltimate();

	void CheckStartWarlockUltimate();
	void WarlockSkillTakeDamageAndHealPlayer(FVector Origin);
	
	UFUNCTION(BlueprintCallable)
	void SpawnShield();

	UFUNCTION(BlueprintCallable)
	void EndShield();

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
	void WarlockSkillStart(float ZDirection, float LaunchStrength, float GravityScale, FVector CameraLocation);

	UFUNCTION(BlueprintCallable)
	void WarlockSkillFall(float ZDirection, float LaunchStrength, float GravityScale);

	UFUNCTION(BlueprintCallable)
	void WarlockSkillLand();

	UFUNCTION(BlueprintCallable)
	void WarlockSkillEnd();

	UFUNCTION(BlueprintCallable)
	void WarlockMeleeStart(FVector CameraLocation);

	UFUNCTION(BlueprintCallable)
	void WarlockMeleeFire();

	UFUNCTION(BlueprintCallable)
	void WarlockMeleeEnd();

	UFUNCTION(BlueprintCallable)
	void WarlockUltimateStart(FVector CameraLocation);

	UFUNCTION(BlueprintCallable)
	void WarlockUltimateCast();

	UFUNCTION(BlueprintCallable)
	void WarlockUltimateEnd();

	UFUNCTION(BlueprintCallable)
	void SwitchToFirstPerson();

	UFUNCTION(BlueprintCallable)
	void SwitchToThirdPerson();

	void SetClassValue();
	void TitanPunchCollisionEvents();
	void PlayerCarryingStart(ACarriableObject* CarriableObject);
	void PlayerCarryingEnd();

	UFUNCTION(BlueprintCallable)
	FVector GetLastPlayerPos() {return LastPlayerPos;};

	UFUNCTION(BlueprintCallable)
	bool GetIsPlayerAlive() {return bIsPlayerAlive;}

	UFUNCTION(BlueprintCallable)
	bool GetIsPlayerCarrying() {return bIsCarrying;}

	UFUNCTION(BlueprintCallable, Category = "HP")
	virtual float TakeDamage(float DamageAmount, 
    FDamageEvent const& DamageEvent, 
    AController* EventInstigator, 
    AActor* DamageCauser)override;

	UFUNCTION()
    void OnSpearOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                             bool bFromSweep, const FHitResult& SweepResult);

	void SetIsInWarlockAura(bool Value) { bIsInWarlockAura = Value;}

	void PerformComboAttack();
	void ResetCombo();
	
	
private:
	float CurSkillCoolTime = SkillCoolTime;
	float CurGrenadeCoolTime = GrenadeCoolTime;
	float CurUltimateCoolTime;
	float CurUltimateDuration = UltimateDuration;
	float CurSmashCoolTime = SmashCoolTime;
	float CurMeleeAttackCoolTime;
	
	float CurComboAttackDelay = 0.f;
	float ComboAttackDelay = 0.4f;

	bool isTitanPunch = false;
	bool isWarlockSkill = false;

	USkeletalMesh* HunterMesh;
	USkeletalMesh* TitanMesh;
	USkeletalMesh* WarlockMesh;

	UStaticMesh* HunterSpearMesh;

	TSubclassOf<UAnimInstance> HunterAnimInstanceClass;
	TSubclassOf<UAnimInstance> TitanAnimInstanceClass;
	TSubclassOf<UAnimInstance> WarlockAnimInstanceClass;

	USkeletalMesh* SelectedMesh = nullptr;
	TSubclassOf<UAnimInstance> SelectedAnimInstanceClass = nullptr;

	FVector WarlockUltimateSpawnLocation;
	
	int jumpCount = 0;
	float DefaultCapsuleHeight;
    float SlideCapsuleHeight;
	float SlideSpeedScale = 2.3f;
	FVector LastPlayerPos;
	float PosTickCoolTime = 400.0f;
	bool bIsPlayerAlive = true;
	bool bIsCarrying = false;
	bool bIsInWarlockAura = false;

	int32 HunterComboStage;
	bool bIsHunterAttacking;
	bool bHasNextComboQueued;
	float ComboInputWindow;
	FTimerHandle ComboResetTimer;
};
