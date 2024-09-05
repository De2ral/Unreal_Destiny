 // Fill out your copyright notice in the Description page of Project Settings.


#include "DestinyFPSBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "COmponents/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Net/UnrealNetwork.h"
#include "Titan_Skill_Barrier.h"
#include "Grenade.h"
#include "Animation/AnimInstance.h"
#include "MyLegacyCameraShake.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Warlock_Melee_Fireball.h"
#include "Warlock_Skill_Ultimate.h"
#include "EngineUtils.h"
#include "Hunter_Skill_SwordAura.h"
#include "ReplicatedObj.h"


// Sets default values
ADestinyFPSBase::ADestinyFPSBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; 
	TppMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TPPMesh"));
	TppMesh->SetOwnerNoSee(true);
	TppMesh->SetupAttachment(RootComponent);

	TppSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPPSpringArm"));
	TppSpringArm->TargetArmLength = 300.0f;
	TppSpringArm->bUsePawnControlRotation = true;
	TppSpringArm->SocketOffset.Z = 150.f;
	TppSpringArm->SetupAttachment(RootComponent);

	TppCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPPCamera"));
	TppCamera->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
	TppCamera->SetupAttachment(TppSpringArm);

	FppCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPPCamera"));
	FppCamera->bUsePawnControlRotation = true;
	FppCamera->SetupAttachment(GetMesh(), FName("Mesh"));

	FppMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPPMesh"));
	FppMesh->SetOnlyOwnerSee(true);
	FppMesh->SetupAttachment(FppCamera);

	SpearMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpearMesh"));

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> SmashParticleAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Skill_RockBurst/P_RBurst_Fire_Charge_Slam_2.P_RBurst_Fire_Charge_Slam_2'"));
	if (SmashParticleAsset.Succeeded())
		TitanUltimateSmashParticle = SmashParticleAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FistParticleAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/ICE/combat/P_MagicSpray_Ice_02.P_MagicSpray_Ice_02'"));
	if (FistParticleAsset.Succeeded())
		TitanUltimateFistParticle = FistParticleAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> LaunchParticleAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/ICE/P_jumpSmash_Spikes_02.P_jumpSmash_Spikes_02'"));
	if (LaunchParticleAsset.Succeeded())
		TitanUltimateLaunchParticle = LaunchParticleAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> TrailParticleAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Ambient/Fire/P_Fire_wallTorch_Blue_noSmoke.P_Fire_wallTorch_Blue_noSmoke'"));
	if (TrailParticleAsset.Succeeded())
		TitanUltimateTrailParticle = TrailParticleAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PunchParticleAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_waterBallHit.P_ky_waterBallHit'"));
	if (PunchParticleAsset.Succeeded())
		TitanPunchParticle = PunchParticleAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PunchStartParticleAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_shotShockwave_2.P_ky_shotShockwave_2'"));
	if (PunchStartParticleAsset.Succeeded())
		TitanPunchStartParticle = PunchStartParticleAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> WarlockSkillStartParticleAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_shotShockwave.P_ky_shotShockwave'"));
	if (WarlockSkillStartParticleAsset.Succeeded())
		WarlockSkillStartParticle = WarlockSkillStartParticleAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> WarlockSkillLandParticleAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Combat_Base/Resurrection/P_Resurrection.P_Resurrection'"));
	if (WarlockSkillLandParticleAsset.Succeeded())
		WarlockSkillLandParticle = WarlockSkillLandParticleAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> SpearParticleAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKwang/FX/Particles/Abilities/Sword/FX/P_Kwang_Sword_Bolts.P_Kwang_Sword_Bolts'"));
	if (SpearParticleAsset.Succeeded())
		SpearParticle = SpearParticleAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> SpearAttackParticleAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKwang/FX/Particles/Abilities/LightStrike/FX/P_LightStrikeImpact.P_LightStrikeImpact'"));
	if (SpearAttackParticleAsset.Succeeded())
		SpearAttackParticle = SpearAttackParticleAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HunterPunchParticleAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_hit1.P_ky_hit1'"));
	if (HunterPunchParticleAsset.Succeeded())
		HunterPunchParticle = HunterPunchParticleAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HunterThunderPunchParticleAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Lightning/P_LineToPoint_Spawn_Proj_Lightning_00.P_LineToPoint_Spawn_Proj_Lightning_00'"));
	if (HunterThunderPunchParticleAsset.Succeeded())
		HunterThunderPunchParticle = HunterThunderPunchParticleAsset.Object;

	TitanPunchCollider = CreateDefaultSubobject<USphereComponent>(TEXT("TitanPunchCollider"));
	TitanPunchCollider->SetupAttachment(TppMesh, TEXT("TitanUltimateFistSocket"));
	TitanPunchCollider->SetGenerateOverlapEvents(true);
	TitanPunchCollider->InitSphereRadius(0.5f);

	WarlockSkillCollider = CreateDefaultSubobject<USphereComponent>(TEXT("WarlockSkillCollider"));
    WarlockSkillCollider->SetupAttachment(TppMesh, TEXT("GroundSocket"));
	WarlockSkillCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WarlockSkillCollider->InitSphereRadius(5.0f); 

	ConstructorHelpers::FObjectFinder<USkeletalMesh> HunterMeshAsset(
		TEXT("/Script/Engine.SkeletalMesh'/Game/ThirdPerson/Characters/Hunter/Meshes/Genji_Street_Runner.Genji_Street_Runner'"));
	if(HunterMeshAsset.Succeeded())
		HunterMesh = HunterMeshAsset.Object;

	ConstructorHelpers::FClassFinder<UAnimInstance> HunterAnimClassAsset(
		TEXT("/Script/Engine.AnimBlueprint'/Game/ThirdPerson/Characters/Hunter/Animations/ABP_Hunter.ABP_Hunter_C'"));
	if (HunterAnimClassAsset.Succeeded())
		HunterAnimInstanceClass = HunterAnimClassAsset.Class;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TitanMeshAsset(
		TEXT("/Script/Engine.SkeletalMesh'/Game/ThirdPerson/Characters/Titan/Meshes/Omega_Knight.Omega_Knight'"));
	if(TitanMeshAsset.Succeeded())
		TitanMesh = TitanMeshAsset.Object;

	ConstructorHelpers::FClassFinder<UAnimInstance> TitanAnimClassAsset(
		TEXT("/Script/Engine.AnimBlueprint'/Game/ThirdPerson/Characters/Titan/Animations/ABP_Titan.ABP_Titan_C'"));
	if(TitanAnimClassAsset.Succeeded())
		TitanAnimInstanceClass = TitanAnimClassAsset.Class;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> WarlockMeshAsset(
		TEXT("/Script/Engine.SkeletalMesh'/Game/ThirdPerson/Characters/Warlock/Meshes/Warlock.Warlock'"));
	if(WarlockMeshAsset.Succeeded())
		WarlockMesh = WarlockMeshAsset.Object;

	ConstructorHelpers::FClassFinder<UAnimInstance> WarlockAnimClassAsset(
		TEXT("/Script/Engine.AnimBlueprint'/Game/ThirdPerson/Characters/Warlock/Animations/ABP_Warlock.ABP_Warlock_C'"));
	if(WarlockAnimClassAsset.Succeeded())
		WarlockAnimInstanceClass = WarlockAnimClassAsset.Class;

	ConstructorHelpers::FObjectFinder<UStaticMesh> HunterSpearAsset(
		TEXT("/Script/Engine.StaticMesh'/Game/Skill/Hunter/Staff/Spear.Spear'"));
	if(HunterSpearAsset.Succeeded())
		HunterSpearMesh = HunterSpearAsset.Object;

	ConstructorHelpers::FObjectFinder<UAnimMontage> HunterMontageAsset(
		TEXT("/Script/Engine.AnimMontage'/Game/ThirdPerson/Characters/Hunter/Animations/Ultimate_Attack.Ultimate_Attack'"));
    if (HunterMontageAsset.Succeeded())
        HunterComboMontage = HunterMontageAsset.Object;

	PlayerClass = EPlayerClassEnum::HUNTER;

	LastPlayerPos = GetActorLocation();

	HunterComboStage = 0;
	bIsHunterAttacking = false;
	isHasNexCombo = false;
	ComboInputWindow = 0.6f;
}

// Called when the game starts or when spawned
void ADestinyFPSBase::BeginPlay()
{
	Super::BeginPlay();

	SetClassValue();

	FInputModeGameOnly GameOnly;

	// 초기 캡슐 크기 설정
    //GetCapsuleComponent()->InitCapsuleSize(42.0f, DefaultCapsuleHeight);

	// 캐릭터 기본 캡슐 높이 설정
    DefaultCapsuleHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

    // 슬라이딩 시 캡슐 높이 설정 (기본 높이의 절반 또는 사용자가 원하는 값으로 설정)
    SlideCapsuleHeight = DefaultCapsuleHeight / 10.0f;

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController != nullptr)
	{
		PlayerController->SetInputMode(GameOnly);
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if(Subsystem != nullptr)
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			UE_LOG(LogTemp, Warning, TEXT("mapping")); 
		}
	}

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	CurSkillCoolTime = SkillCoolTime;
	CurGrenadeCoolTime = GrenadeCoolTime;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this,&ADestinyFPSBase::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this,&ADestinyFPSBase::OnOverlapEnd);

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
			HUDWidget->UpdateAmmo(WeaponComponent->CurrentAmmo(), WeaponComponent->StoredAmmo());
			HUDWidget->UpdateSkillCoolTime(CurSkillCoolTime, SkillCoolTime);
			HUDWidget->UpdateGrenadeCoolTime(CurGrenadeCoolTime, GrenadeCoolTime);
		}
	}

	SwitchToFirstPerson();
}


void ADestinyFPSBase::SetClassValue()
{
	switch (PlayerClass)
	{
		case EPlayerClassEnum::HUNTER:
			{
				// Set Character Mesh & Anim Blueprint by Class
				if (TppMesh)
				{
					if (HunterMesh)
						TppMesh->SetSkeletalMesh(HunterMesh);
					if (HunterAnimInstanceClass)
						TppMesh->SetAnimInstanceClass(HunterAnimInstanceClass);
				}

				if (SpearMesh)
				{
					if (HunterSpearMesh)
						SpearMesh->SetStaticMesh(HunterSpearMesh);
					if (TppMesh)
					{
						FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
       					SpearMesh->AttachToComponent(TppMesh, AttachmentRules, FName(TEXT("SpearSocket")));
						SpearMesh->SetVisibility(false);
					}
					SpearMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
   					SpearMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    				SpearMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
					SpearMesh->OnComponentBeginOverlap.AddDynamic(this, &ADestinyFPSBase::OnSpearOverlap);
					//SpearMesh->SetIsReplicated(true);
				}

				// Set Character Movement by Player Class
				this->JumpMaxCount = 2;

				// Set Character Skill Value by Player Class
				GrenadeCoolTime = 3.f;
				UltimateCoolTime = 40.f;
				UltimateDuration = 30.f;
				MeleeAttackCoolTime = 2.f;
				HunterMeleeAttackCoolTime = 8.f;
			}
		break;

		case EPlayerClassEnum::TITAN:
			{
				// Set Character Mesh & Anim Blueprint by Class
				if (TppMesh)
				{
					if (TitanMesh)
						TppMesh->SetSkeletalMesh(TitanMesh);
					if (TitanAnimInstanceClass)
						TppMesh->SetAnimInstanceClass(TitanAnimInstanceClass);
				}

				// Set Character Movement by Player Class
				this->JumpMaxCount = 1;

				// Set Character Skill Value by Player Class
				SkillCoolTime = 3.f;
				GrenadeCoolTime = 3.f;
				UltimateCoolTime = 40.f;
				UltimateDuration = 30.f;
				SmashCoolTime = 7.5f;
				MeleeAttackCoolTime = 8.f;
			}
		break;

		case EPlayerClassEnum::WARLOCK:
			{
				// Set Character Mesh & Anim Blueprint by Class
				if (TppMesh)
				{
					if (WarlockMesh)
						TppMesh->SetSkeletalMesh(WarlockMesh);
					if (WarlockAnimInstanceClass)
						TppMesh->SetAnimInstanceClass(WarlockAnimInstanceClass);
				}

				// Set Character Movement by Player Class
				this->JumpMaxCount = 1;
				GetCharacterMovement()->JumpZVelocity = 800.0f;

				// Set Character Skill Value by Player Class
				SkillCoolTime = 3.f;
				GrenadeCoolTime = 3.f;
				UltimateCoolTime = 60.f;
				MeleeAttackCoolTime = 10.f;
			}
		break;

		default:
			{

			}
		break;
	}
	CurSkillCoolTime = SkillCoolTime;
	CurGrenadeCoolTime = GrenadeCoolTime;
	CurUltimateCoolTime = UltimateCoolTime;
	CurUltimateDuration = UltimateDuration;
	CurSmashCoolTime = SmashCoolTime;
	CurMeleeAttackCoolTime = MeleeAttackCoolTime;
	CurHunterMeleeAttackCoolTime = HunterMeleeAttackCoolTime;
	CurSwordAuraCoolTime = SwordAuraCoolTime;
}

void ADestinyFPSBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (CurSkillCoolTime < SkillCoolTime)
		{
			CurSkillCoolTime += DeltaTime;
			HUDWidget->UpdateSkillCoolTime(CurSkillCoolTime, SkillCoolTime);
		}

		if (CurGrenadeCoolTime < GrenadeCoolTime)
		{
			CurGrenadeCoolTime += DeltaTime;
			HUDWidget->UpdateGrenadeCoolTime(CurGrenadeCoolTime, GrenadeCoolTime);
		}

		if (CurUltimateCoolTime < UltimateCoolTime)
		{
			CurUltimateCoolTime += DeltaTime;
		}

		if (CurUltimateDuration < UltimateDuration)
		{
			CurUltimateDuration += DeltaTime;
			if (CurUltimateDuration >= UltimateDuration)
				EndUltimate();
		}

		if (CurSmashCoolTime < SmashCoolTime)
		{
			CurSmashCoolTime += DeltaTime;
		}

		if (CurMeleeAttackCoolTime < MeleeAttackCoolTime)
		{
			CurMeleeAttackCoolTime += DeltaTime;
		}

		if (CurHunterMeleeAttackCoolTime < HunterMeleeAttackCoolTime)
		{
			CurHunterMeleeAttackCoolTime += DeltaTime;
		}

		if (CurSwordAuraCoolTime < SwordAuraCoolTime)
		{
			CurSwordAuraCoolTime += DeltaTime;
		}
	}

	if (isTitanPunch)
	{
		TitanPunchCollisionEvents();
	}
	//슬라이딩 시작 (마지막으로 받은 전방벡터로 고정)
	if(bIsSliding)
	{
		AddMovementInput(SlideVector,1);
		SlideTime -= 1;

	}

	//슬라이딩 끝
	if(SlideTime < 0.0f)
	{
		bIsSliding = false;
		SlideTime = 150.0f;
		GetCharacterMovement()->MaxWalkSpeed /= SlideSpeedScale;
  		GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHeight);
	}

	if(InterObj && bIsInteractComplete)
	{
		
		InterObjAction();
		bIsInteractComplete = false;
	} 

	//->AddOnScreenDebugMessage(-1,1.0f,FColor::Cyan,FString::Printf("MaxWalkSpeed = %f",GetCharacterMovement()->MaxWalkSpeed));

	if(PosTickCoolTime > 0.0f) PosTickCoolTime -= 1;

	else if(PosTickCoolTime <= 0.0f && !GetCharacterMovement()->IsFalling())
	{
		if(bIsPlayerAlive)
		{
			LastPlayerPos = GetActorLocation();

			GEngine->AddOnScreenDebugMessage(
				-1,
				1.5f,
				FColor::Blue,
				FString::Printf(TEXT("LastPlayerPos = X=%f,Y=%f,Z=%f"),LastPlayerPos.X,LastPlayerPos.Y,LastPlayerPos.Z));

			PosTickCoolTime = 400.0f;

		}
	} 

	if (CurComboAttackDelay > 0.f)
		CurComboAttackDelay -= DeltaTime;
}

void ADestinyFPSBase::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (Input != nullptr)
	{
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADestinyFPSBase::Look);
		Input->BindAction(DeathReviveAction, ETriggerEvent::Started, this, &ADestinyFPSBase::HPDamageTest);

		if(bIsPlayerAlive)
		{
			Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADestinyFPSBase::Move);

			Input->BindAction(SkillAction, ETriggerEvent::Started, this, &ADestinyFPSBase::Skill);
			Input->BindAction(GrenadeAction, ETriggerEvent::Started, this, &ADestinyFPSBase::Grenade);
			Input->BindAction(MeleeAttackAction, ETriggerEvent::Started, this, &ADestinyFPSBase::MeleeAttack);
			Input->BindAction(UltimateAction, ETriggerEvent::Started, this, &ADestinyFPSBase::Ultimate);
			
			Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ADestinyFPSBase::jump);
			Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADestinyFPSBase::jumpEnd);

			Input->BindAction(SprintAction, ETriggerEvent::Started, this, &ADestinyFPSBase::Sprint);
			Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &ADestinyFPSBase::SprintEnd);

			Input->BindAction(SlideAction, ETriggerEvent::Started, this, &ADestinyFPSBase::Slide);
			//Input->BindAction(SlideAction, ETriggerEvent::Completed, this, &ADestinyFPSBase::SlideEnd);

			Input->BindAction(LeftClickAction,ETriggerEvent::Started, this, &ADestinyFPSBase::LeftClickFunction);
			Input->BindAction(RightClickAction,ETriggerEvent::Started, this, &ADestinyFPSBase::RightClickFunction);
			
			Input->BindAction(InterAction, ETriggerEvent::Triggered, this, &ADestinyFPSBase::StartInteract);
			Input->BindAction(InterAction, ETriggerEvent::Completed, this, &ADestinyFPSBase::EndInteract);

			Input->BindAction(InventoryAction,ETriggerEvent::Completed, this, &ADestinyFPSBase::InvenOpenClose);
		}
		
	}
}

void ADestinyFPSBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ADestinyFPSBase, SpearMesh);
    
    DOREPLIFETIME(ADestinyFPSBase, HP);    
	DOREPLIFETIME(ADestinyFPSBase, bHasRifle);
	DOREPLIFETIME(ADestinyFPSBase, isSkill);   
	DOREPLIFETIME(ADestinyFPSBase, isGrenade);   
	DOREPLIFETIME(ADestinyFPSBase, isUltimate);   
	DOREPLIFETIME(ADestinyFPSBase, isSmash);   
	DOREPLIFETIME(ADestinyFPSBase, isMeleeAttack);   
	DOREPLIFETIME(ADestinyFPSBase, isSwordAura);  

	DOREPLIFETIME(ADestinyFPSBase, CurSkillCoolTime);   
	DOREPLIFETIME(ADestinyFPSBase, CurGrenadeCoolTime);   
	DOREPLIFETIME(ADestinyFPSBase, CurUltimateCoolTime);   
	DOREPLIFETIME(ADestinyFPSBase, CurUltimateDuration);   
	DOREPLIFETIME(ADestinyFPSBase, CurSmashCoolTime);   
	DOREPLIFETIME(ADestinyFPSBase, CurMeleeAttackCoolTime);   
	DOREPLIFETIME(ADestinyFPSBase, CurHunterMeleeAttackCoolTime); 

	DOREPLIFETIME(ADestinyFPSBase, SpawnedDeathOrb);   
}

void ADestinyFPSBase::InvenOpenClose()
{
	if(bIsInvenOpen)
	{
		bIsInvenOpen = false;
		GEngine->AddOnScreenDebugMessage(-1,0.5f,FColor::Red,TEXT("인벤토리가 닫혔다"));

	}

	else if(!bIsInvenOpen)
	{
		bIsInvenOpen = true;
		GEngine->AddOnScreenDebugMessage(-1,0.5f,FColor::Red,TEXT("인벤토리가 열렸다"));
	}


}

void ADestinyFPSBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (!isSmash && !isMeleeAttack && !isSkill && !bIsHunterAttacking)
	{
		if (Controller != nullptr && !bIsSliding && bIsPlayerAlive)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			AddMovementInput(FowardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void ADestinyFPSBase::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (Controller != nullptr)
	{
		if(PlayerController->IsInputKeyDown(EKeys::RightMouseButton))
		{
			AddControllerYawInput(LookVector.X * 0.1f);
			AddControllerPitchInput(LookVector.Y * 0.1f);
		}
		else
		{
			AddControllerYawInput(LookVector.X);
			AddControllerPitchInput(LookVector.Y);
		}
	}
}

void ADestinyFPSBase::Skill()
{
	if(bIsCarrying || !bIsPlayerAlive) return;
	if(HasAuthority())
	{
		isSkill = true;
		CurSkillCoolTime = 0.f;
		if ((PlayerClass == EPlayerClassEnum::TITAN) || 
		(PlayerClass == EPlayerClassEnum::WARLOCK && GetCharacterMovement()->IsFalling()))
		{
			WeaponComponent->SetCurrentWeaponMeshVisibility(false);
			SwitchToThirdPerson();
		}
	}
	else
	{
		Server_Skill(true);
	}
}

void ADestinyFPSBase::SpawnShield()
{
	if (!HasAuthority()) return;
	UWorld* world = GetWorld();

	if(world)
	{
		FActorSpawnParameters SpawnParams;
		FVector SpawnLocation = this->GetActorLocation() + this->GetActorRotation().Vector() * 100.f;
		SpawnLocation.Z += 20.f;
		FRotator SpawnRotation = this->GetActorRotation();
		ATitan_Skill_Barrier* skillObject = GetWorld()->SpawnActor<ATitan_Skill_Barrier>(ATitan_Skill_Barrier::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void ADestinyFPSBase::EndShield()
{
	if (HasAuthority())
	{
		isSkill = false;
		SwitchToFirstPerson();
		WeaponComponent->SetCurrentWeaponMeshVisibility(true);
	}
	else
	{
		Server_Skill(false);
	}
}

void ADestinyFPSBase::Grenade()
{
	if(bIsCarrying) return;
	if (HasAuthority())
	{
		isGrenade = true;
		CurGrenadeCoolTime = 0.f;
	}
	else
	{
		Server_Grenade(true);
	}
}

void ADestinyFPSBase::Throw()
{
	if(!HasAuthority()) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("쿨타임 : %f"), CurGrenadeCoolTime));

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	UWorld* world = GetWorld();
	if (world && PlayerController)
	{
		FActorSpawnParameters SpawnParams;
		FVector SpawnLocation = FppMesh->GetSocketLocation("GripPoint");
		SpawnLocation.Z -= 50.f; 
		FRotator SpawnRotation = this->GetActorRotation();
		AGrenade* grenadeObject = GetWorld()->SpawnActor<AGrenade>(GrenadeClass, SpawnLocation, SpawnRotation, SpawnParams);
		grenadeObject->SetThrowDirection(PlayerController->GetControlRotation().Vector() + this->GetActorUpVector() / 10.f);
	}
}

void ADestinyFPSBase::SwitchToFirstPerson()
{
	if (FppCamera && TppCamera)
	{
		FppCamera->SetActive(true);
		TppCamera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		TppCamera->SetActive(false);
	}

	if (FppMesh && TppMesh)
	{
		FppMesh->SetOwnerNoSee(false);
		FppMesh->SetOnlyOwnerSee(true);

		TppMesh->SetOwnerNoSee(true);
	}
	WeaponComponent->SetCurrentWeaponMeshVisibility(true);
}

void ADestinyFPSBase::SwitchToThirdPerson()
{
	if (FppCamera && TppCamera)
	{
		FppCamera->SetActive(false);
		TppCamera->SetActive(true);
	}

	if (FppMesh && TppMesh)
	{
		FppMesh->SetOwnerNoSee(true);
		FppMesh->SetOnlyOwnerSee(false);

		TppMesh->SetOwnerNoSee(false);
	}
}

void ADestinyFPSBase::PlayerCarryingStart(AReplicatedObj* CarriableObject)
{
	 if (!CarriableObject)
    {
        return; // 운반할 대상이 없으면 종료
    }

    bIsCarrying = true;
	SwitchToThirdPerson();

    // CarriableObject의 메쉬 정보를 가져옴
	UStaticMesh* CarriableMesh = CarriableObject->GetObjMesh()->GetStaticMesh();

    // UStaticMeshComponent 생성
    CarriedMeshComponent = NewObject<UStaticMeshComponent>(this);
    if (CarriedMeshComponent)
    {
        // 가져온 메쉬를 컴포넌트에 설정
        CarriedMeshComponent->SetStaticMesh(CarriableMesh);

        // 생성된 컴포넌트를 액터에 부착
        CarriedMeshComponent->AttachToComponent(TppMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("CarriableSocket"));
        
		// 스케일 조정
        FVector DesiredScale(0.3f, 0.3f, 0.3f);
        CarriedMeshComponent->SetWorldScale3D(DesiredScale);

        // 컴포넌트 활성화 및 렌더링 설정
        CarriedMeshComponent->RegisterComponent();
        CarriedMeshComponent->SetVisibility(true);
    }

}

void ADestinyFPSBase::PlayerCarryingEnd()
{
	 if (bIsCarrying)
    {
        bIsCarrying = false;

		SwitchToFirstPerson();

		if(CarriedMeshComponent)
		{

			// 플레이어 앞쪽 위치 계산
			FVector PlayerLocation = GetActorLocation();
        	FRotator PlayerRotation = GetActorRotation();

        	FVector ForwardVector = PlayerRotation.Vector();
        	FVector DropLocation = PlayerLocation + ForwardVector * 100.0f; // 플레이어 앞쪽 100 유닛

        	// ACarriableObject를 플레이어 앞에 스폰
        	FActorSpawnParameters SpawnParams;
        	AReplicatedObj* DroppedObject = GetWorld()->SpawnActor<AReplicatedObj>(AReplicatedObj::StaticClass(), DropLocation, PlayerRotation, SpawnParams);

        	// DroppedObject에 원래 메쉬 설정
        	if (DroppedObject && DroppedObject->GetObjMesh())
        	{
        	    DroppedObject->GetObjMesh()->SetStaticMesh(CarriedMeshComponent->GetStaticMesh());
        	}

			// 기존에 붙어있던 컴포넌트 삭제
			CarriedMeshComponent->DestroyComponent();
        	CarriedMeshComponent = nullptr;
		}

    }

}

void ADestinyFPSBase::MeleeAttack()
{
	if(HasAuthority())
	{
		if (CurMeleeAttackCoolTime >= MeleeAttackCoolTime)
		{
			isMeleeAttack = true;
			CurMeleeAttackCoolTime = 0.f;
			WeaponComponent->SetCurrentWeaponMeshVisibility(false);
			SwitchToThirdPerson();
			if(CurHunterMeleeAttackCoolTime >= HunterMeleeAttackCoolTime)
			{
				isHunterMeleeAttack = true;
				CurHunterMeleeAttackCoolTime = 0.f;
			}
		}
	}
	else
	{
		Server_MeleeAttack(true);
	}
}

void ADestinyFPSBase::Ultimate()
{
	if(bIsCarrying) return;
	if(HasAuthority())
	{
		isUltimate = true;
		CurUltimateCoolTime = 0.f;
		CurUltimateDuration = 0.f;
		SwitchToThirdPerson();
		GEngine->AddOnScreenDebugMessage(-1,30.0f,FColor::Blue, TEXT("Ultimate if문 진입"));
		if (PlayerClass == EPlayerClassEnum::TITAN)
		{
			GEngine->AddOnScreenDebugMessage(-1,30.0f,FColor::Blue, TEXT("Switching to Third Person"));
			MeleeAttackCoolTime = 2.f;
			WeaponComponent->SetCurrentWeaponMeshVisibility(false);
			// Titan Ultimate Start
			if (TitanUltimateFistParticle)
			{
				UGameplayStatics::SpawnEmitterAttached(
					TitanUltimateFistParticle,
					TppMesh,
					FName("TitanUltimateFistSocket"),
					FVector::ZeroVector, 
					FRotator::ZeroRotator,
					EAttachLocation::SnapToTarget,
					true
				);
			}
		}
		else if (PlayerClass == EPlayerClassEnum::WARLOCK)
		{
			CheckStartWarlockUltimate();
		}
		else if (PlayerClass == EPlayerClassEnum::HUNTER)
		{
			CurUltimateCoolTime = 0.f;
			CurUltimateDuration = 0.f;
			WeaponComponent->SetCurrentWeaponMeshVisibility(false);
			GEngine->AddOnScreenDebugMessage(-1,30.0f,FColor::Cyan,TEXT("헌터 궁극기 시전. (서버)"));
			Multicast_UpdateSpearMeshVisibility(isUltimate);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,30.0f,FColor::Cyan,TEXT("헌터 궁극기 시전. (클라)"));
		Server_Ultimate(true);
	}
}

void ADestinyFPSBase::CheckStartWarlockUltimate()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if (PlayerController)
	{
		FVector StartLocation;
		FRotator StartRotation;

		PlayerController->GetPlayerViewPoint(StartLocation, StartRotation);
		FVector EndLocation = StartLocation + (StartRotation.Vector() * MaxSpawnWarlockUltimateDistance);

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECC_Visibility,
			QueryParams
		);

		if (bHit)
		{
			if (HitResult.ImpactNormal.Z > 0.99f)
			{
				float Distance = FVector::Dist(StartLocation, HitResult.Location);
				if (Distance <= MaxSpawnWarlockUltimateDistance)
				{
					WeaponComponent->SetCurrentWeaponMeshVisibility(false);
					SwitchToThirdPerson();
					WarlockUltimateSpawnLocation = HitResult.Location;
				}
			}
		}
	}
}

void ADestinyFPSBase::CameraShake(float Scale)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());

	if (PlayerController && TppCamera)
	{ 
		if (PlayerController->IsLocalController())
		{
			PlayerController->ClientStartCameraShake(UMyLegacyCameraShake::StaticClass(), Scale);
		}
	}
}

void ADestinyFPSBase::TitanMeleeAttackStart(float ZDirection, float LaunchStrength)
{
	FVector LaunchDirection = GetActorRotation().Vector();
	LaunchDirection.Z += ZDirection;
	FVector LaunchVelocity = LaunchDirection * LaunchStrength;
	LaunchCharacter(LaunchVelocity, true, true);
	isTitanPunch = true;

	if (TitanUltimateFistParticle)
	{
		FVector ParticleSpawnLocation = TppMesh->GetSocketLocation(TEXT("TitanUltimateFistSocket"));
		FRotator ParticleSpawnRotation = this->GetActorRotation();
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            TitanPunchStartParticle,
            ParticleSpawnLocation,
            ParticleSpawnRotation,
			(FVector)((1.5F))
        );
	}
}

void ADestinyFPSBase::TitanMeleeAttackEnd()
{
	if (HasAuthority())
	{
		isMeleeAttack = false;
		isTitanPunch = false;
		if (!isUltimate)
		{
			WeaponComponent->SetCurrentWeaponMeshVisibility(true);
			SwitchToFirstPerson();
		}
	}
	else
	{
		Server_MeleeAttack(false);
	}
}

void ADestinyFPSBase::TitanSmashStart(float ZDirection, float LaunchStrength, float GravityScale, FVector CameraLocation)
{
	FVector LaunchDirection = GetActorRotation().Vector();
	LaunchDirection.Z += ZDirection;
	FVector LaunchVelocity = LaunchDirection * LaunchStrength;
	LaunchCharacter(LaunchVelocity, true, true);
	GetCharacterMovement()->GravityScale = GravityScale;

	TppSpringArm->TargetArmLength = 500.0f;
	TppCamera->SetRelativeLocation(CameraLocation);

	if (TitanUltimateLaunchParticle)
    {
		FVector ParticleSpawnLocation = TppMesh->GetSocketLocation(TEXT("GroundSocket"));
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            TitanUltimateLaunchParticle,
            ParticleSpawnLocation,
            FRotator(0.f, 0.f, 0.f),
			(FVector)((1.5F))
        );
    }

	if (TitanUltimateTrailParticle)
    {
        UGameplayStatics::SpawnEmitterAttached(
            TitanUltimateTrailParticle,
            TppMesh,
			FName("RootSocket"),
            FVector::ZeroVector, 
            FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
        );
    }
}

void ADestinyFPSBase::TitanSmashTop(float ZDirection, float LaunchStrength, float GravityScale)
{
	FVector LaunchDirection = GetActorRotation().Vector();
	LaunchDirection.Z += ZDirection;
	FVector LaunchVelocity = LaunchDirection * LaunchStrength;
	LaunchCharacter(LaunchVelocity, true, true);
	GetCharacterMovement()->GravityScale = GravityScale;
}

void ADestinyFPSBase::TitanSmashDown()
{
	// VFX & Camera Moving
	CameraShake(3.f);
	if (TitanUltimateSmashParticle)
    {
		FVector ParticleSpawnLocation = TppMesh->GetSocketLocation(TEXT("TitanUltimateFistSocket"));
		ParticleSpawnLocation.Z -= 100.f;
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            TitanUltimateSmashParticle,
            ParticleSpawnLocation,
            FRotator(0.f, 0.f, 0.f),
			(FVector)((1.5F))
        );
    }

	// Apply Damage
	UGameplayStatics::ApplyRadialDamage(
		this,
		TitanUltimateDamage,
		GetActorLocation(),
		SmashRadius,
		UDamageType::StaticClass(),
		TArray<AActor*>(),
		this,
		GetInstigatorController(),
		true
	);

	DrawDebugSphere(GetWorld(), GetActorLocation(), SmashRadius, 12, FColor::Green, false, 1.0f);
}

void ADestinyFPSBase::TitanSmashEnd(float DelayTime)
{
	GetCharacterMovement()->GravityScale = 1.f;
	TppCamera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	TppSpringArm->TargetArmLength = 300.0f;
	isSmash = false;
}

void ADestinyFPSBase::WarlockSkillStart(float ZDirection, float LaunchStrength, float GravityScale, FVector CameraLocation)
{
	isWarlockSkill = false;

	FVector LaunchDirection = GetActorRotation().Vector();
	LaunchDirection.Z += ZDirection;
	FVector LaunchVelocity = LaunchDirection * LaunchStrength;
	LaunchCharacter(LaunchVelocity, true, true);
	GetCharacterMovement()->GravityScale = GravityScale;

	TppCamera->SetRelativeLocation(CameraLocation);

	FVector ParticleSpawnLocation = this->GetActorLocation();
	FRotator ParticleSpawnRotation = this->GetActorRotation();
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		WarlockSkillStartParticle,
		ParticleSpawnLocation,
		ParticleSpawnRotation,
		(FVector)((1.5F))
	);
}

void ADestinyFPSBase::WarlockSkillFall(float ZDirection, float LaunchStrength, float GravityScale)
{
	FVector LaunchDirection = GetActorRotation().Vector();
	LaunchDirection.Z += ZDirection;
	FVector LaunchVelocity = LaunchDirection * LaunchStrength;
	LaunchCharacter(LaunchVelocity, true, true);
	GetCharacterMovement()->GravityScale = GravityScale;
}

void ADestinyFPSBase::WarlockSkillLand()
{
	CameraShake(1.5f);
	FVector ParticleSpawnLocation = TppMesh->GetSocketLocation(TEXT("GroundSocket"));

	FVector Start = FVector(ParticleSpawnLocation.X, ParticleSpawnLocation.Y, 10000.0f);
    FVector End = FVector(ParticleSpawnLocation.X, ParticleSpawnLocation.Y, -10000.0f);
    FHitResult HitResult;
    FCollisionQueryParams Params;
    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
        ParticleSpawnLocation.Z = HitResult.Location.Z;
	FRotator ParticleSpawnRotation = FRotator(0.f, 0.f, 0.f);
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		WarlockSkillLandParticle,
		ParticleSpawnLocation,
		ParticleSpawnRotation,
		(FVector)((1.5F))
	);

	WarlockSkillTakeDamageAndHealPlayer(GetActorLocation());
}

void ADestinyFPSBase::WarlockSkillTakeDamageAndHealPlayer(FVector Origin)
{
	if (!isWarlockSkill)
	{
		TArray<AActor*> OverlappingActors;
		WarlockSkillCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
   		WarlockSkillCollider->GetOverlappingActors(OverlappingActors);

		for (AActor* Actor : OverlappingActors)
		{
			if (Actor)
			{
				// 액터가 플레이어 클래스인지 확인
				if (Actor->IsA(ADestinyFPSBase::StaticClass()))
				{
					// 플레이어 클래스의 경우 체력 회복
					ADestinyFPSBase* Player = Cast<ADestinyFPSBase>(Actor);
					if (Player)
						UGameplayStatics::ApplyDamage(Player, -WarlockSkillHealAmount, GetInstigatorController(), this, nullptr);
				}
				else
				{
					// 일반 액터에게 데미지 적용
					UGameplayStatics::ApplyDamage(
						Actor,
						WarlockSkillDamage,
						GetInstigatorController(),
						this,
						UDamageType::StaticClass()
					);
				}
			}
		}
		WarlockSkillCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ADestinyFPSBase::WarlockSkillEnd()
{
	if (HasAuthority())
	{
		isSkill = false;
		GetCharacterMovement()->GravityScale = 1.f;
		SwitchToFirstPerson();
		WeaponComponent->SetCurrentWeaponMeshVisibility(true);
	}
	else
	{
		Server_Skill(false);
	}
}

void ADestinyFPSBase::WarlockMeleeStart(FVector CameraLocation)
{
	TppCamera->SetRelativeLocation(CameraLocation);
}

void ADestinyFPSBase::WarlockMeleeFire()
{
	if(!HasAuthority()) return;
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;  // 플레이어 앞에서 스폰

    TArray<float> Angles = { -30.f, -15.f, 0.f, 15.f, 30.f };

    /*APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
    if (!PlayerController) return;

	ADestinyFPSBase* Player = Cast<ADestinyFPSBase>(PlayerController->GetPawn());
    if (!Player) return;

	UCameraComponent* CameraComponent = Player->FindComponentByClass<UCameraComponent>();
    if (!CameraComponent) return;*/

	FVector CameraForward = FppCamera->GetForwardVector();

    for (float Angle : Angles)
    {
        FRotator ShootRotation = CameraForward.Rotation();
        ShootRotation.Yaw += Angle;

        AWarlock_Melee_Fireball* Fireball = GetWorld()->SpawnActor<AWarlock_Melee_Fireball>(WarlockFireballClass, SpawnLocation, ShootRotation);

        if (Fireball)
        {
            FVector ShootDirection = ShootRotation.Vector(); 
            Fireball->SetFireballDirection(ShootDirection);
        }
    }
}

void ADestinyFPSBase::WarlockMeleeEnd()
{
	if (HasAuthority())
	{
		isMeleeAttack = false;
		SwitchToFirstPerson();
	}
	else
	{
		Server_MeleeAttack(false);
	}
}

void ADestinyFPSBase::WarlockUltimateStart(FVector CameraLocation)
{
	TppCamera->SetRelativeLocation(CameraLocation);
}

void ADestinyFPSBase::WarlockUltimateCast()
{
	UWorld* world = GetWorld();
	if (world)
	{
		FActorSpawnParameters SpawnParams;
		FRotator SpawnRotation = FRotator::ZeroRotator;
		AWarlock_Skill_Ultimate* WarlockUltimateObject = 
			GetWorld()->SpawnActor<AWarlock_Skill_Ultimate>(WarlockUltimateClass, WarlockUltimateSpawnLocation, SpawnRotation, SpawnParams);
	}
}

void ADestinyFPSBase::WarlockUltimateEnd()
{
	if (HasAuthority())
	{
		isUltimate = false;
		SwitchToFirstPerson();
	}
	else
	{
		Server_Ultimate(false);
	}
}

void ADestinyFPSBase::HunterSwordAura()
{
	if(!HasAuthority()) return;
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;

    TArray<float> Angles = { -30.f, -15.f, 0.f, 15.f, 30.f };
	FVector CameraForward = TppCamera->GetForwardVector();
	FRotator ShootRotation = this->GetActorRotation();

	AHunter_Skill_SwordAura* swordAura = GetWorld()->SpawnActor<AHunter_Skill_SwordAura>(AHunter_Skill_SwordAura::StaticClass(), SpawnLocation, ShootRotation);

	if (swordAura)
	{
		FVector ShootDirection = CameraForward; 
		swordAura->SetSwordAuraDirection(ShootDirection);
	}
}

void ADestinyFPSBase::HunterSwordAuraEnd()
{
	if (HasAuthority())
	{
		isSwordAura = false;
	}
	else
	{
		Server_SwordAura(false);
	}
}

void ADestinyFPSBase::HunterMeleePunch()
{
	CameraShake(2.f);

	FVector ParticleSpawnLocation = TppMesh->GetSocketLocation(TEXT("PunchSocket"));
	FRotator ParticleSpawnRotation = FRotator(0.f, 0.f, 0.f);
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		HunterPunchParticle,
		ParticleSpawnLocation,
		ParticleSpawnRotation,
		(FVector)((0.5F))
	);

	float Damage = HunterPunchDamage;
	if (isHunterMeleeAttack)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HunterThunderPunchParticle,
			ParticleSpawnLocation,
			ParticleSpawnRotation,
			(FVector)((0.5F))
		);
		isHunterMeleeAttack = false;
		Damage = HunterPunchDamage * 2;
	}

	UGameplayStatics::ApplyRadialDamage(
		this,
		Damage,
		ParticleSpawnLocation,
		HunterPunchRadius,
		UDamageType::StaticClass(),
		TArray<AActor*>(),
		this,
		GetInstigatorController(),
		true
	);
}

void ADestinyFPSBase::HunterMeleeEnd()
{
	if (HasAuthority())
	{
		isMeleeAttack = false;
		SwitchToFirstPerson();
	}
	else
	{
		Server_MeleeAttack(false);
	}
}

void ADestinyFPSBase::EndUltimate()
{
	if (HasAuthority())
	{
		isUltimate = false;
		if (PlayerClass == EPlayerClassEnum::TITAN)
		{
			isMeleeAttack = false;
			isSmash = false;
			MeleeAttackCoolTime = 8.f;
		}
		if (PlayerClass == EPlayerClassEnum::HUNTER)
		{
			HunterComboStage = 0;
			bIsHunterAttacking = false;
			isHasNexCombo = false;
			Multicast_UpdateSpearMeshVisibility(false);
		}
		SwitchToFirstPerson();
		WeaponComponent->SetCurrentWeaponMeshVisibility(true);
	}
	else
	{
		Server_Ultimate(false);
	}
}

void ADestinyFPSBase::jump(const FInputActionValue& Value)
{
	ACharacter::Jump();
	UE_LOG(LogTemp, Warning, TEXT("jump")); 

	if(PlayerClass == EPlayerClassEnum::WARLOCK)
	{
		if(GetCharacterMovement()->IsFalling())
		{
			GetCharacterMovement()->GravityScale = 0.2f;
		}

	}
}

void ADestinyFPSBase::jumpEnd(const FInputActionValue &Value)
{
	ACharacter::StopJumping();

	if(PlayerClass == EPlayerClassEnum::WARLOCK)
	{
		if(GetCharacterMovement()->IsFalling())
		{
			GetCharacterMovement()->GravityScale = 1.0f;
		}

	}
}

void ADestinyFPSBase::Sprint(const FInputActionValue& Value)
{
	if(bIsCarrying) return;

	bPlayerSprint = true;
	GetCharacterMovement()->MaxWalkSpeed *= 1.5f;

	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Blue,TEXT("bPlayerSprint = true"));

}

void ADestinyFPSBase::SprintEnd(const FInputActionValue& Value)
{
	if(bIsCarrying) return;
	
	bPlayerSprint = false;
	GetCharacterMovement()->MaxWalkSpeed /= 1.5f;

	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Blue,TEXT("bPlayerSprint = false"));

}

void ADestinyFPSBase::Slide(const FInputActionValue& Value)
{
	//SlidingTime만큼 슬라이딩 시작
	
	if(!bIsSliding)
	{
		SlideVector = FppCamera->GetForwardVector();
		bIsSliding = true;
		GetCharacterMovement()->MaxWalkSpeed *= SlideSpeedScale;
		GetCapsuleComponent()->SetCapsuleHalfHeight(SlideCapsuleHeight);
		PlayerCarryingEnd();
	}

}

 //void ADestinyFPSBase::SlideEnd(const FInputActionValue& Value)
 //{
 //	GetCharacterMovement()->MaxWalkSpeed /= 2.0f;
//	bIsSliding = false;
 //   GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHeight);
 //}

void ADestinyFPSBase::StartInteract(const FInputActionValue &Value)
{
	if(bPlayerInteractable && InteractTime <= MaxInteractTime)
	{
		//GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("bPlayerIntaractable = true"));
		GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Blue,FString::Printf(TEXT("InteractTime = %f"),InteractTime));
		InteractTime += 1.0f;

	}

	if(InteractTime > MaxInteractTime)
	{
		bIsInteractComplete = true;
		InteractTime = 0.0f;
		bPlayerInteractable = false;
	}

}

void ADestinyFPSBase::EndInteract(const FInputActionValue &Value)
{
	if(!bPlayerInteractable)
	{
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("bPlayerIntaractable = false"));
		//bIsInteractComplete = false;
	}
	else if(bPlayerInteractable)
	{
		InteractTime = 0.0f;
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("InteractTime to 0"));
		//bIsInteractComplete = false;
	}

}

void ADestinyFPSBase::LeftClickFunction(const FInputActionValue &Value)
{
	if (isUltimate)
	{
		if (PlayerClass == EPlayerClassEnum::TITAN)
		{
			if (HasAuthority())
			{
				if (CurMeleeAttackCoolTime >= MeleeAttackCoolTime)
				{
					isMeleeAttack = true;
					CurMeleeAttackCoolTime = 0.f;
				}
			}
			else
			{
				Server_MeleeAttack(true);
			}
		}
		if (PlayerClass == EPlayerClassEnum::HUNTER)
		{
			if (HasAuthority())
            {
                if (bIsHunterAttacking && CurComboAttackDelay <= 0.f)
                {
                    isHasNexCombo = true;
                    PerformComboAttack();
                }
                else
                {
                    PerformComboAttack();
                }
            }
            else
            {
                Server_PerformComboAttack(HunterComboStage);
            }
		}
	}
}

void ADestinyFPSBase::RightClickFunction(const FInputActionValue &Value)
{
	if(bIsCarrying) return;
	if (isUltimate)
	{
		if (PlayerClass == EPlayerClassEnum::TITAN)
		{
			if (HasAuthority())
			{
				if (CurSmashCoolTime >= SmashCoolTime)
				{
					isSmash = true;
					CurSmashCoolTime = 0.f;
				}
			}
			else
			{
				Server_Smash(true);
			}
		}
		if (PlayerClass == EPlayerClassEnum::HUNTER)
		{
			if (HasAuthority())
			{
				if (CurSwordAuraCoolTime >= SwordAuraCoolTime)
				{
					isSwordAura = true;
					CurSwordAuraCoolTime = 0.f;
				}
			}
			else
			{
				Server_SwordAura(true);
			}
		}
	}
}

void ADestinyFPSBase::PerformComboAttack()
{
	UAnimInstance* AnimInstance = TppMesh->GetAnimInstance();
    if (AnimInstance)
    {
        // Determine the next combo stage
        if ((isHasNexCombo) && (HunterComboStage < 5))
        {
            HunterComboStage++;
            isHasNexCombo = false;
            CurComboAttackDelay = ComboAttackDelay;
        }
        else if (!bIsHunterAttacking)
        {
            HunterComboStage = 1;
            CurComboAttackDelay = ComboAttackDelay;
        }
        else
        {
            // If no input was queued, exit the function
            return;
        }

        if (HasAuthority())
        {
            // On server: play montage and notify all clients
            PlayMontage_Internal(HunterComboStage);
            Multicast_PlayComboMontage(HunterComboStage);
        }
        else
        {
            // On client: request server to perform the combo attack
            Server_PerformComboAttack(HunterComboStage);
        }

        bIsHunterAttacking = true;
        GetWorldTimerManager().SetTimer(ComboResetTimer, this, &ADestinyFPSBase::ResetCombo, ComboInputWindow, false);
    }
}

void ADestinyFPSBase::PlayMontage_Internal(int32 ComboStage)
{
	UAnimInstance* AnimInstance = TppMesh->GetAnimInstance();
    if (AnimInstance && HunterComboMontage)
    {
        FName ComboSectionName = FName(*FString::Printf(TEXT("Combo%d"), ComboStage));
        if (!AnimInstance->Montage_IsPlaying(HunterComboMontage))
        {
            AnimInstance->Montage_Play(HunterComboMontage, 1.f);
        }
        AnimInstance->Montage_JumpToSection(ComboSectionName, HunterComboMontage);
		isSpearAttack = true;
    }
}

void ADestinyFPSBase::ResetCombo()
{
    // Reset combo variables
    bIsHunterAttacking = false;
    HunterComboStage = 0;
    isHasNexCombo = false;
    GetWorldTimerManager().ClearTimer(ComboResetTimer);
}

void ADestinyFPSBase::Death()
{
	bIsPlayerAlive = false;

	if(!SpawnedDeathOrb)
	{
		SpawnedDeathOrb = GetWorld()->SpawnActor<AReplicatedObj>(AReplicatedObj::StaticClass(),LastPlayerPos,GetActorRotation());

		SpawnedDeathOrb->Tags.Add(FName("InterObj"));
		SpawnedDeathOrb->Tags.Add(FName("DeathOrb"));
		SetActorLocation(LastPlayerPos);
		SwitchToThirdPerson();

		TppMesh->SetOwnerNoSee(true);
		FppMesh->SetOwnerNoSee(true);

		if(bIsCarrying) PlayerCarryingEnd();

		SpawnedDeathOrb->SetDeadPlayer(this);
        
    }
}

void ADestinyFPSBase::Revive()
{
	bIsPlayerAlive = true;

	if(SpawnedDeathOrb != nullptr)
	{
		SpawnedDeathOrb->Destroy();
		SpawnedDeathOrb = nullptr;
	}

	SwitchToFirstPerson();

}

void ADestinyFPSBase::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

void ADestinyFPSBase::TitanPunchCollisionEvents()
{
	TArray<AActor*> OverlappingActors;
	TitanPunchCollider->GetOverlappingActors(OverlappingActors);

	if (OverlappingActors.Num() > 1)
	{
		CameraShake(1.5f);
		TArray<AActor*> IgnoredActors;

		for (TActorIterator<ADestinyFPSBase> It(GetWorld()); It; ++It)
		{
			AActor* ActorToIgnore = *It;
			IgnoredActors.Add(ActorToIgnore);
		}

		isTitanPunch = false;

		UGameplayStatics::ApplyRadialDamage(
			this,
			TitanPunchDamage,
			GetActorLocation(),
			TitanPunchRadius,
			nullptr,  // 데미지 타입
			IgnoredActors,  // 무시할 액터들
			this,
			GetInstigatorController(),
			true  // Do full damage
    	);

		if (TitanPunchParticle)
		{
			FVector ParticleSpawnLocation = TppMesh->GetSocketLocation(TEXT("TitanUltimateFistSocket"));
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				TitanPunchParticle,
				ParticleSpawnLocation,
				FRotator(0.f, 0.f, 0.f),
				(FVector)((1.5F))
			);
		}
	}
}

float ADestinyFPSBase::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{	
    //if(!bCanTakeDamage) return 0.0f;
    
	float Damage = Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator, DamageCauser);

    HP -= (bIsInWarlockAura && DamageAmount > 0) ? (DamageAmount * 0.8) : DamageAmount;
    
	if(bIsPlayerAlive && HP <= 0.0f) Death();
	//else if (!bIsPlayerAlive && HP > 0.0f) Revive();

    return Damage;
}

void ADestinyFPSBase::HPDamageTest(const FInputActionValue &Value)
{
	if(HP > 0.0f) HP -= 10.0f;
	else if(HP <= 0.0f) Death();
}

void ADestinyFPSBase::OnSpearOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (isUltimate && bIsHunterAttacking && isSpearAttack)
	{
		if (OtherActor && !OtherActor->IsA(ADestinyFPSBase::StaticClass()))
		{
			CameraShake(1.f);
			UGameplayStatics::ApplyDamage(OtherActor, HunterUltimateAttackDamage, GetController(), this, nullptr);
			FVector ParticleSpawnLocation = OtherComp->GetComponentLocation();
			FRotator ParticleSpawnRotation = FRotator(0.f, 0.f, 0.f);
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				SpearAttackParticle,
				ParticleSpawnLocation,
				ParticleSpawnRotation,
				(FVector)((1.5F))
			);
			isSpearAttack = false;
		}
	}
}

void ADestinyFPSBase::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->ActorHasTag("InteractObj") || OtherActor->ActorHasTag("NPC"))
    {
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Yellow,TEXT("isObj"));
        InterObj = Cast<AReplicatedObj>(OtherActor);

        if (bIsPlayerAlive)
        {
            bPlayerInteractable = true;
            MaxInteractTime = InterObj->ObjInteractTime;
        }
        
    }

}

void ADestinyFPSBase::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("InteractObj") || OtherActor->ActorHasTag("NPC"))
    {
		if (bIsPlayerAlive) bPlayerInteractable = false;
	
	}


}

void ADestinyFPSBase::Server_Skill_Implementation(bool value)
{
	if (value)
	{
		if (CurSkillCoolTime >= SkillCoolTime)
		{
			isSkill = value;
			CurSkillCoolTime = 0.f;
		}
	}
	else
	{
		isSkill = value;
	}
}

bool ADestinyFPSBase::Server_Skill_Validate(bool value)
{
    return true;
}

void ADestinyFPSBase::Server_Ultimate_Implementation(bool value)
{
	GEngine->AddOnScreenDebugMessage(-1,30.0f,FColor::Cyan,TEXT("서버 궁극기 함수 실행"));
	if (value)
	{
		if (CurUltimateCoolTime >= UltimateCoolTime)
		{
			isUltimate = value;
			CurUltimateCoolTime = 0.f;
			CurUltimateDuration = 0.f;
			Multicast_UpdateSpearMeshVisibility(isUltimate);
		}
	}
	else
	{
		isUltimate = value;
		if (PlayerClass == EPlayerClassEnum::TITAN)
		{
			isMeleeAttack = false;
			isSmash = false;
			MeleeAttackCoolTime = 8.f;
		}
		if (PlayerClass == EPlayerClassEnum::HUNTER)
		{
			HunterComboStage = 0;
			bIsHunterAttacking = false;
			isHasNexCombo = false;
			Multicast_UpdateSpearMeshVisibility(false);
		}
	}
}

bool ADestinyFPSBase::Server_Ultimate_Validate(bool value)
{
    return true;
}

void ADestinyFPSBase::Server_MeleeAttack_Implementation(bool value)
{
	if (value)
	{
		if (CurMeleeAttackCoolTime >= MeleeAttackCoolTime)
		{
			isMeleeAttack = value;
			CurMeleeAttackCoolTime = 0.f;
			if(CurHunterMeleeAttackCoolTime >= HunterMeleeAttackCoolTime)
			{
				isHunterMeleeAttack = true;
				CurHunterMeleeAttackCoolTime = 0.f;
			}
		}
	}
	else
	{
		isMeleeAttack = value;
		isTitanPunch = value;
	}
}

bool ADestinyFPSBase::Server_MeleeAttack_Validate(bool value)
{
    return true;
}

void ADestinyFPSBase::Server_Grenade_Implementation(bool value)
{
	if (value)
	{
		if (CurGrenadeCoolTime >= GrenadeCoolTime)
		{
			isGrenade = value;
			CurGrenadeCoolTime = 0.f;
		}
	}
	else
	{
		isGrenade = value;
	}
}

void ADestinyFPSBase::ServerInterObjAction_Implementation()
{

	InterObjAction();

}

void ADestinyFPSBase::MultiInterObjAction_Implementation()
{

	InterObj->GetObjMesh()->SetStaticMesh(InterObj->AfterInteractMesh->GetStaticMesh());

}



bool ADestinyFPSBase::ServerInterObjAction_Validate()
{
    return true;
}


bool ADestinyFPSBase::Server_Grenade_Validate(bool value)
{
    return true;
}

void ADestinyFPSBase::Server_Smash_Implementation(bool value)
{
	if (value)
	{
		if (CurSmashCoolTime >= SmashCoolTime)
		{
			isSmash = value;
			CurSmashCoolTime = 0.f;
		}
	}
	else
	{
		isSmash = value;
	}
}

bool ADestinyFPSBase::Server_Smash_Validate(bool value)
{
    return true;
}

void ADestinyFPSBase::Server_PerformComboAttack_Implementation(int32 ComboStage)
{
	if (bIsHunterAttacking && CurComboAttackDelay <= 0.f)
	{
		isHasNexCombo = true;
		PerformComboAttack();
	}
	else
	{
		PerformComboAttack();
	}
}

bool ADestinyFPSBase::Server_PerformComboAttack_Validate(int32 ComboStage)
{
    return true;
}

void ADestinyFPSBase::Server_SwordAura_Implementation(bool value)
{
	if (value)
	{
		if (CurSwordAuraCoolTime >= SwordAuraCoolTime)
		{
			isSwordAura = value;
			CurSwordAuraCoolTime = 0.f;
		}
	}
	else
	{
		isSwordAura = value;
	}
}

bool ADestinyFPSBase::Server_SwordAura_Validate(bool value)
{
    return true;
}
void ADestinyFPSBase::InterObjAction()
{
	if(HasAuthority())
	{	
		if(InterObj->ActorHasTag("Stash"))
		{
			int32 ItemCount = FMath::RandRange(InterObj->MinItemValue,InterObj->MaxItemValue);
			InterObj->ItemDrop(ItemCount);
			InterObj->Destroy();
		}

		if(InterObj->ActorHasTag("NPC")) InterObj->ShowQuestUI();

		if(InterObj->ActorHasTag("Carriable"))
		{
			if(!bIsCarrying) PlayerCarryingStart(InterObj);
			SwitchToThirdPerson();
			InterObj->Destroy();
		}

		if(InterObj->ActorHasTag("CarryInput"))
		{
			if(bIsCarrying)
			{
				CarriedMeshComponent->DestroyComponent();
				CarriedMeshComponent = nullptr;
				PlayerCarryingEnd();
				InterObj->SetObjIsFill(true);
			}

		}

		if(InterObj->ActorHasTag("DeathOrb"))
		{
			InterObj->GetDeadPlayer()->Revive();
			InterObj->Destroy();
		}

	} else ServerInterObjAction();
}

void ADestinyFPSBase::OnRep_Skill()
{
	if (isSkill)
	{
		if ((PlayerClass == EPlayerClassEnum::TITAN) || 
		(PlayerClass == EPlayerClassEnum::WARLOCK && GetCharacterMovement()->IsFalling()))
		{
			WeaponComponent->SetCurrentWeaponMeshVisibility(false);
			SwitchToThirdPerson();
		}
	}
	else
	{
		SwitchToFirstPerson();
		WeaponComponent->SetCurrentWeaponMeshVisibility(true);
	}
}

void ADestinyFPSBase::OnRep_Ultimate()
{
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, TEXT("OnRep_Ultimate 호출됨"));
	if (isUltimate)
	{
		GEngine->AddOnScreenDebugMessage(-1,30.0f,FColor::Blue, TEXT("Ultimate if문 진입"));
		if (PlayerClass == EPlayerClassEnum::TITAN)
		{
			GEngine->AddOnScreenDebugMessage(-1,30.0f,FColor::Blue, TEXT("Switching to Third Person"));
			MeleeAttackCoolTime = 2.f;
			WeaponComponent->SetCurrentWeaponMeshVisibility(false);
			SwitchToThirdPerson();
			// Titan Ultimate Start
			if (TitanUltimateFistParticle)
			{
				UGameplayStatics::SpawnEmitterAttached(
					TitanUltimateFistParticle,
					TppMesh,
					FName("TitanUltimateFistSocket"),
					FVector::ZeroVector, 
					FRotator::ZeroRotator,
					EAttachLocation::SnapToTarget,
					true
				);
			}
		}
		else if (PlayerClass == EPlayerClassEnum::WARLOCK)
		{
			CheckStartWarlockUltimate();
		}
		else if (PlayerClass == EPlayerClassEnum::HUNTER)
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, TEXT("OnRep_Ultimate: SpearMesh visible"));
			WeaponComponent->SetCurrentWeaponMeshVisibility(false);
			SwitchToThirdPerson();
		}
	}
	else
	{
		SwitchToFirstPerson();
		WeaponComponent->SetCurrentWeaponMeshVisibility(true);
	}
}	

void ADestinyFPSBase::OnRep_MeleeAttack()
{
	if (isMeleeAttack)
	{
		WeaponComponent->SetCurrentWeaponMeshVisibility(false);
		SwitchToThirdPerson();
	}
	else
	{
		if (!isUltimate)
		{
			WeaponComponent->SetCurrentWeaponMeshVisibility(true);
			SwitchToFirstPerson();
		}
	}
}

void ADestinyFPSBase::Multicast_UpdateSpearMeshVisibility_Implementation(bool bVisible)
{
	if (SpearMesh)
	{
		if (bVisible)
		{
			HunterSpearSpawnedEmitter =
			UGameplayStatics::SpawnEmitterAttached(
				SpearParticle,
				TppMesh,
				FName("SpearEffectSocket"),
				FVector::ZeroVector, 
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				true
			);
		}
		else
		{
			HunterSpearSpawnedEmitter->DestroyComponent();
		}
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, FString::Printf(TEXT("Multicast_SetStaticMeshVisibility called with bVisible: %d"), bVisible));
		SpearMesh->SetVisibility(bVisible, true);
	}
}

void ADestinyFPSBase::Multicast_PlayComboMontage_Implementation(int32 ComboStage)
{
	PlayMontage_Internal(ComboStage);
}