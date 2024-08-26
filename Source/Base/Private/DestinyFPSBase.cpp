// Fill out your copyright notice in the Description page of Project Settings.


#include "DestinyFPSBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Titan_Skill_Barrier.h"
#include "Titan_Skill_Grenade.h"


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
	TppSpringArm->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
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

	PlayerClass = EPlayerClassEnum::Warlock;

	LastPlayerPos = GetActorLocation();

	switch(PlayerClass)
	{
		case EPlayerClassEnum::Hunter:
			this->JumpMaxCount = 2;
			break;
		case EPlayerClassEnum::Titan:
			this->JumpMaxCount = 1;
			break;
		case EPlayerClassEnum::Warlock:
			this->JumpMaxCount = 1;
			GetCharacterMovement()->JumpZVelocity = 800.0f;
			break;
		default:
			break;
	}
}

// Called when the game starts or when spawned
void ADestinyFPSBase::BeginPlay()
{
	Super::BeginPlay();

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

void ADestinyFPSBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsPlayerAlive && HP <= 0.0f) Death();
	else if (!bIsPlayerAlive && HP > 0.0f) Revive();

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

	//GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Cyan,FString::Printf("MaxWalkSpeed = %f",GetCharacterMovement()->MaxWalkSpeed));

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

			//if(DeathOrbTest) GetWorld()->SpawnActor<AActor>(DeathOrbTest,LastPlayerPos,GetActorRotation());

		}
		
	} 
	
	
}

void ADestinyFPSBase::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (Input != nullptr)
	{
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADestinyFPSBase::Look);
		Input->BindAction(DeathReviveAction, ETriggerEvent::Started, this, &ADestinyFPSBase::DeathRevive);

		if(bIsPlayerAlive)
		{
			Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADestinyFPSBase::Move);
			Input->BindAction(SkillAction, ETriggerEvent::Started, this, &ADestinyFPSBase::Skill);
			Input->BindAction(GrenadeAction, ETriggerEvent::Started, this, &ADestinyFPSBase::Grenade);
			Input->BindAction(UltimateAction, ETriggerEvent::Started, this, &ADestinyFPSBase::Ultimate);
			Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ADestinyFPSBase::jump);
			Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADestinyFPSBase::jumpEnd);

			Input->BindAction(SprintAction, ETriggerEvent::Started, this, &ADestinyFPSBase::Sprint);
			Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &ADestinyFPSBase::SprintEnd);

			Input->BindAction(SlideAction, ETriggerEvent::Started, this, &ADestinyFPSBase::Slide);
			//Input->BindAction(SlideAction, ETriggerEvent::Completed, this, &ADestinyFPSBase::SlideEnd);

			Input->BindAction(InterAction, ETriggerEvent::Triggered, this, &ADestinyFPSBase::StartInteract);
			Input->BindAction(InterAction, ETriggerEvent::Completed, this, &ADestinyFPSBase::EndInteract);

			Input->BindAction(InventoryAction,ETriggerEvent::Completed, this, &ADestinyFPSBase::InvenOpenClose);
		}
		
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

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
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

		TppMesh->SetOnlyOwnerSee(false);
		TppMesh->SetOwnerNoSee(false);
	}
}

void ADestinyFPSBase::Ultimate(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("궁극기"));

	SwitchToThirdPerson();
}
void ADestinyFPSBase::jump(const FInputActionValue& Value)
{
	ACharacter::Jump();
	UE_LOG(LogTemp, Warning, TEXT("jump")); 

	if(PlayerClass == EPlayerClassEnum::Warlock)
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

	if(PlayerClass == EPlayerClassEnum::Warlock)
	{
		if(GetCharacterMovement()->IsFalling())
		{
			GetCharacterMovement()->GravityScale = 1.0f;
		}

	}
}

void ADestinyFPSBase::Sprint(const FInputActionValue& Value)
{
	bPlayerSprint = true;
	GetCharacterMovement()->MaxWalkSpeed *= 1.5f;

	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Blue,TEXT("bPlayerSprint = true"));

}

void ADestinyFPSBase::SprintEnd(const FInputActionValue& Value)
{
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

void ADestinyFPSBase::Death()
{
	bIsPlayerAlive = false;

	if(!SpawnedDeathOrb)
	{
		SpawnedDeathOrb = GetWorld()->SpawnActor<AActor>(DeathOrbTest,LastPlayerPos,GetActorRotation());
		SetActorLocation(LastPlayerPos);
		SwitchToThirdPerson();
		TppMesh->SetOwnerNoSee(true);
		FppMesh->SetOwnerNoSee(true);
        
    }
}

void ADestinyFPSBase::Revive()
{
	bIsPlayerAlive = true;

	//사망 테스트를 위해 추가한 기능 (추후 멀티플레이 구현 시 삭제 요망)
	if(SpawnedDeathOrb != nullptr) SpawnedDeathOrb->Destroy();

	SwitchToFirstPerson();

}

void ADestinyFPSBase::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

void ADestinyFPSBase::DeathRevive(const FInputActionValue &Value)
{
	if(HP > 0.0f) HP = 0.0f;
	else if(HP <= 0.0f) HP = 100.0f;

}