// Fill out your copyright notice in the Description page of Project Settings.


#include "DestinyFPSBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Titan_Skill_Barrier.h"
#include "Titan_Skill_Grenade.h"
#include "Animation/AnimInstance.h"
#include "MyLegacyCameraShake.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ADestinyFPSBase::ADestinyFPSBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
		

	TitanSmashCollider = CreateDefaultSubobject<USphereComponent>(TEXT("TitanSmashCollider"));
	TitanSmashCollider->SetupAttachment(TppMesh, TEXT("GroundSocket"));
	TitanSmashCollider->SetGenerateOverlapEvents(true);
	TitanSmashCollider->InitSphereRadius(10.f);

	TitanPunchCollider = CreateDefaultSubobject<USphereComponent>(TEXT("TitanPunchCollider"));
	TitanPunchCollider->SetupAttachment(TppMesh, TEXT("TitanUltimateFistSocket"));
	TitanPunchCollider->SetGenerateOverlapEvents(true);
	TitanPunchCollider->InitSphereRadius(0.5f);

	TitanPunchDamageCollider = CreateDefaultSubobject<USphereComponent>(TEXT("TitanPunchDamageCollider"));
	TitanPunchDamageCollider->SetupAttachment(TppMesh, TEXT("TitanUltimateFistSocket"));
	TitanPunchDamageCollider->SetGenerateOverlapEvents(true);
	TitanPunchDamageCollider->InitSphereRadius(2.f);

	PlayerClass = EPlayerClassEnum::TITAN;

	SetClassValue();
}

// Called when the game starts or when spawned
void ADestinyFPSBase::BeginPlay()
{
	Super::BeginPlay();

	if (TppMesh)
	{
		if (SelectedMesh)
			TppMesh->SetSkeletalMesh(SelectedMesh);
		if (SelectedAnimInstanceClass)
			TppMesh->SetAnimInstanceClass(SelectedAnimInstanceClass);
	}

	FInputModeGameOnly GameOnly;

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

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
			HUDWidget->UpdateAmmo(WeaponComponent->CurrentAmmo, WeaponComponent->MaxAmmo);
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
				ConstructorHelpers::FObjectFinder<USkeletalMesh> HunterMeshAsset(
					TEXT("/Script/Engine.SkeletalMesh'/Game/ThirdPerson/Characters/Hunter/Meshes/Genji_Street_Runner.Genji_Street_Runner'"));
				if(HunterMeshAsset.Succeeded())
					SelectedMesh = HunterMeshAsset.Object;

				ConstructorHelpers::FClassFinder<UAnimInstance> HunterAnimClassAsset(
					TEXT("/Script/Engine.AnimBlueprint'/Game/ThirdPerson/Characters/Hunter/Animations/ABP_Hunter.ABP_Hunter_C'"));
				if (HunterAnimClassAsset.Succeeded())
				{
					SelectedAnimInstanceClass = HunterAnimClassAsset.Class;
				}
			}
		break;

		case EPlayerClassEnum::TITAN:
			{
				ConstructorHelpers::FObjectFinder<USkeletalMesh> TitanMeshAsset(
					TEXT("/Script/Engine.SkeletalMesh'/Game/ThirdPerson/Characters/Titan/Meshes/Omega_Knight.Omega_Knight'"));
				if(TitanMeshAsset.Succeeded())
					SelectedMesh = TitanMeshAsset.Object;

				ConstructorHelpers::FClassFinder<UAnimInstance> TitanAnimClassAsset(
					TEXT("/Script/Engine.AnimBlueprint'/Game/ThirdPerson/Characters/Titan/Animations/ABP_Titan.ABP_Titan_C'"));
				if(TitanAnimClassAsset.Succeeded())
					SelectedAnimInstanceClass = TitanAnimClassAsset.Class;
			}
		break;

		case EPlayerClassEnum::WARLOCK:
			{
			}
		break;

		default:
			{

			}
		break;
	}
}

void ADestinyFPSBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	if (isTitanPunch)
	{
		TitanPunchCollisionEvents();
	}
}

void ADestinyFPSBase::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (Input != nullptr)
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADestinyFPSBase::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADestinyFPSBase::Look);
		Input->BindAction(SkillAction, ETriggerEvent::Started, this, &ADestinyFPSBase::Skill);
		Input->BindAction(GrenadeAction, ETriggerEvent::Started, this, &ADestinyFPSBase::Grenade);
		Input->BindAction(UltimateAction, ETriggerEvent::Started, this, &ADestinyFPSBase::Ultimate);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ADestinyFPSBase::jump);

		Input->BindAction(SprintAction, ETriggerEvent::Started, this, &ADestinyFPSBase::Sprint);
		Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &ADestinyFPSBase::SprintEnd);

		Input->BindAction(SlideAction, ETriggerEvent::Triggered, this, &ADestinyFPSBase::Slide);
		Input->BindAction(SlideAction, ETriggerEvent::Completed, this, &ADestinyFPSBase::SlideEnd);

		Input->BindAction(InterAction, ETriggerEvent::Triggered, this, &ADestinyFPSBase::StartInteract);
		Input->BindAction(InterAction, ETriggerEvent::Completed, this, &ADestinyFPSBase::EndInteract);

		Input->BindAction(InventoryAction,ETriggerEvent::Completed, this, &ADestinyFPSBase::InvenOpenClose);

		Input->BindAction(LeftClickAction,ETriggerEvent::Started, this, &ADestinyFPSBase::LeftClickFunction);
		Input->BindAction(RightClickAction,ETriggerEvent::Started, this, &ADestinyFPSBase::RightClickFunction);
	}
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

	if (!isSmash && !isMeleeAttack)
	{
		if (Controller != nullptr)
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

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void ADestinyFPSBase::Skill(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("전용 스킬"));

	if(CurSkillCoolTime >= SkillCoolTime)
	{
		CurSkillCoolTime = 0.f;
		isShield = true;
	}
}

void ADestinyFPSBase::Shield()
{
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

void ADestinyFPSBase::Grenade(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("쿨타임 : %f"), CurGrenadeCoolTime));
	if (CurGrenadeCoolTime >= GrenadeCoolTime)
	{
		CurGrenadeCoolTime = 0.f;
		isGrenade = true;
	}
}

void ADestinyFPSBase::Throw()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("쿨타임 : %f"), CurGrenadeCoolTime));

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	UWorld* world = GetWorld();
	if (world && PlayerController)
	{
		FActorSpawnParameters SpawnParams;
		FVector SpawnLocation = FppMesh->GetSocketLocation("GripPoint");
		SpawnLocation.Z -= 50.f; 
		FRotator SpawnRotation = this->GetActorRotation();
		ATitan_Skill_Grenade* grenadeObject = GetWorld()->SpawnActor<ATitan_Skill_Grenade>(GrenadeClass, SpawnLocation, SpawnRotation, SpawnParams);
		grenadeObject->SetThrowDirection(PlayerController->GetControlRotation().Vector() + this->GetActorUpVector() / 10.f);
	}
}

void ADestinyFPSBase::SwitchToFirstPerson()
{
	if (FppCamera && TppCamera)
	{
		FppCamera->SetActive(true);
		TppCamera->SetActive(false);
	}

	if (FppMesh && TppMesh)
	{
		FppMesh->SetOwnerNoSee(false);
		FppMesh->SetOnlyOwnerSee(true);

		TppMesh->SetOwnerNoSee(true);
		TppMesh->SetOnlyOwnerSee(false);
	}
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
		TppMesh->SetOnlyOwnerSee(true);
	}
}

void ADestinyFPSBase::Ultimate(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("궁극기"));

	if (CurUltimateCoolTime >= UltimateCoolTime)
	{
		CurUltimateCoolTime = 0.f;
		CurUltimateDuration = 0.f;
		isUltimate = true;
		SwitchToThirdPerson();
		if (PlayerClass == EPlayerClassEnum::TITAN)
		{
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
	isMeleeAttack = false;
	isTitanPunch = false;
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
	
	WeaponComponent->SetCurrentWeaponMeshVisibility(false);

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
	TArray<AActor*> OverlappingActors;
	TitanSmashCollider->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor->IsA(ADestinyFPSBase::StaticClass()))
			UGameplayStatics::ApplyDamage(Actor, UltimateDamage, GetInstigatorController(), this, nullptr);
	}
}

void ADestinyFPSBase::TitanSmashEnd(float DelayTime)
{
	GetCharacterMovement()->GravityScale = 1.f;
	TppCamera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	TppSpringArm->TargetArmLength = 300.0f;
	isSmash = false;
}

void ADestinyFPSBase::EndUltimate()
{
	isUltimate = false;
	isMeleeAttack = false;
	isSmash = false;
	SwitchToFirstPerson();
	WeaponComponent->SetCurrentWeaponMeshVisibility(true);
}

void ADestinyFPSBase::jump(const FInputActionValue& Value)
{
	ACharacter::Jump();
	UE_LOG(LogTemp, Warning, TEXT("jump")); 
}

void ADestinyFPSBase::Sprint(const FInputActionValue& Value)
{
	bPlayerSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;

	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Blue,TEXT("bPlayerSprint = true"));

}

void ADestinyFPSBase::SprintEnd(const FInputActionValue& Value)
{
	bPlayerSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;

	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Blue,TEXT("bPlayerSprint = false"));

}

void ADestinyFPSBase::Slide(const FInputActionValue& Value)
{
	ACharacter::Crouch(false);
	GetCharacterMovement()->MaxWalkSpeedCrouched = 3000;
	//FppCamera->SetRelativeLocation(FVector(0,0,-20.0f));

}

void ADestinyFPSBase::SlideEnd(const FInputActionValue& Value)
{
	ACharacter::UnCrouch(false);
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300;
	//FppCamera->SetRelativeLocation(FVector(0,0,20.0f));
}

void ADestinyFPSBase::StartInteract(const FInputActionValue &Value)
{
	if(bPlayerInteractable && InteractTime <= MaxInteractTime)
	{
		//GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("bPlayerIntaractable = true"));
		GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Blue,FString::Printf(TEXT("InteractTime = %f"),InteractTime));
		InteractTime += 1.0f;

	}

}

void ADestinyFPSBase::EndInteract(const FInputActionValue &Value)
{
	if(!bPlayerInteractable)
	{
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("bPlayerIntaractable = false"));

	}
	else if(bPlayerInteractable)
	{
		InteractTime = 0.0f;
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("InteractTime to 0"));
	}

}

void ADestinyFPSBase::LeftClickFunction(const FInputActionValue &Value)
{
	if (isUltimate)
	{
		if (PlayerClass == EPlayerClassEnum::TITAN)
		{
			if (CurMeleeAttackCoolTime >= MeleeAttackCoolTime)
			{
				CurMeleeAttackCoolTime = 0.f;
				isMeleeAttack = true;
			}
		}
	}
}

void ADestinyFPSBase::RightClickFunction(const FInputActionValue &Value)
{
	if (isUltimate)
	{
		if (PlayerClass == EPlayerClassEnum::TITAN)
		{
			if (CurSmashCoolTime >= SmashCoolTime)
			{
				CurSmashCoolTime = 0.f;
				isSmash = true;
			}
		}
	}
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
		
		TArray<AActor*> DamagedActors;
		TitanPunchDamageCollider->GetOverlappingActors(DamagedActors);
		for (AActor* Actor : DamagedActors)
		{
			if (!Actor->IsA(ADestinyFPSBase::StaticClass()))
				UGameplayStatics::ApplyDamage(Actor, TitanPunchDamage, GetInstigatorController(), this, nullptr);
		}
		isTitanPunch = false;

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
