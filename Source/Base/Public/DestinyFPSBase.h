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

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* SpearMesh;

	UPROPERTY(VisibleAnywhere)
	class AReplicatedObj* InterObj;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Replicated)
	bool bIsSliding = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Replicated)
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

	UPROPERTY(EditDefaultsOnly,Replicated)
	FVector SlideVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerClassEnum PlayerClass;

	// Skill Variable
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Skill, EditAnywhere, BlueprintReadWrite)
	bool isSkill = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool isGrenade = false;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Ultimate, EditAnywhere, BlueprintReadWrite)
	bool isUltimate = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool isSmash = false;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_MeleeAttack, EditAnywhere, BlueprintReadWrite)
	bool isMeleeAttack = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool isSwordAura = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool isHunterMeleeAttack = false;

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
	float HunterMeleeAttackCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SwordAuraCoolTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HunterPunchDamage = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HunterPunchRadius = 200.f;

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

	UPROPERTY(Replicated)
	float SlideSpeedScale = 2.3f;

	UPROPERTY(Replicated)
	float SlideCapsuleHeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* SpearParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* SpearAttackParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* HunterPunchParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* HunterThunderPunchParticle;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<class AHunter_Skill_SwordAura> HunterSwordAuraClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	bool bIsInvenOpen = false;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	bool bPlayerIsMoving = false;

	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadOnly)
	class AReplicatedObj* SpawnedDeathOrb;
	
	void InvenOpenClose();

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
	void Skill();
	void Grenade();
	void MeleeAttack();

	void jump(const FInputActionValue& Value);
	void jumpEnd(const FInputActionValue& Value);
	void Ultimate();

	void Sprint(const FInputActionValue& Value);
	void SprintEnd(const FInputActionValue& Value);

	void Slide(const FInputActionValue& Value);
	
	void OnRep_Slide();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
	void HunterSwordAura();

	UFUNCTION(BlueprintCallable)
	void HunterSwordAuraEnd();

	UFUNCTION(BlueprintCallable)
	void HunterMeleePunch();

	UFUNCTION(BlueprintCallable)
	void HunterMeleeEnd();

	UFUNCTION(BlueprintCallable)
	void SwitchToFirstPerson();

	UFUNCTION(BlueprintCallable)
	void SwitchToThirdPerson();

	void SetClassValue();
	void TitanPunchCollisionEvents();
	void PlayerCarryingStart(AReplicatedObj* CarriableObject);
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
    void OnSpearOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetIsInWarlockAura(bool Value) { bIsInWarlockAura = Value;}

	void PerformComboAttack();
	void PlayMontage_Internal(int32 ComboStage);
	void ResetCombo();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Skill(bool value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Ultimate(bool value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MeleeAttack(bool value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Grenade(bool value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Smash(bool value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PerformComboAttack(int32 ComboStage);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SwordAura(bool value);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInterObjAction();

	UFUNCTION(NetMulticast, Reliable)
	void MultiInterObjAction();

	void InterObjAction();

	UFUNCTION()
	void OnRep_Skill();

	UFUNCTION()
	void OnRep_Ultimate();

	UFUNCTION()
	void OnRep_MeleeAttack();

	UPROPERTY(Replicated, BlueprintReadOnly)
	float CurSkillCoolTime;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float CurGrenadeCoolTime;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float CurUltimateCoolTime;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float CurUltimateDuration;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float CurSmashCoolTime;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float CurMeleeAttackCoolTime;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float CurSwordAuraCoolTime;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float CurHunterMeleeAttackCoolTime;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateSpearMeshVisibility(bool bVisible);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayComboMontage(int32 ComboStage);
	
private:
	float CurComboAttackDelay = 0.f;
	float ComboAttackDelay = 0.4f;

	bool isTitanPunch = false;
	bool isWarlockSkill = false;
	bool isSpearAttack = false;

	USkeletalMesh* HunterMesh;
	USkeletalMesh* TitanMesh;
	USkeletalMesh* WarlockMesh;

	UStaticMesh* HunterSpearMesh;
	UParticleSystemComponent* HunterSpearSpawnedEmitter;

	TSubclassOf<UAnimInstance> HunterAnimInstanceClass;
	TSubclassOf<UAnimInstance> TitanAnimInstanceClass;
	TSubclassOf<UAnimInstance> WarlockAnimInstanceClass;

	USkeletalMesh* SelectedMesh = nullptr;
	TSubclassOf<UAnimInstance> SelectedAnimInstanceClass = nullptr;

	FVector WarlockUltimateSpawnLocation;
	
	int jumpCount = 0;
	float DefaultCapsuleHeight;
	FVector LastPlayerPos;
	float PosTickCoolTime = 400.0f;
	bool bIsPlayerAlive = true;
	bool bIsCarrying = false;
	bool bIsInWarlockAura = false;

	int32 HunterComboStage;
	bool bIsHunterAttacking;
	bool isHasNexCombo;
	float ComboInputWindow;
	FTimerHandle ComboResetTimer;
};
