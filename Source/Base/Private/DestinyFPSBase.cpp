// Fill out your copyright notice in the Description page of Project Settings.


#include "DestinyFPSBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
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

	FppMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPPMesh"));
	TppMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TPPMesh"));

	FppMesh->SetOnlyOwnerSee(true);
	TppMesh->SetOwnerNoSee(true);

	TppSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPPSpringArm"));

	FppCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPPCamera"));
	TppCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPPCamera"));

	TppSpringArm->SetupAttachment(RootComponent);
	TppCamera->SetupAttachment(TppSpringArm);
	TppMesh->SetupAttachment(RootComponent);

	FppCamera->SetupAttachment(GetMesh(), FName("Mesh"));
	FppMesh->SetupAttachment(FppCamera);

	TppSpringArm->TargetArmLength = 300.0f;
	TppSpringArm->bUsePawnControlRotation = true;
	TppSpringArm->SetRelativeLocation(FVector(0.f, 0.f, 70.f));

	
	FppCamera->bUsePawnControlRotation = true;

	TppCamera->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
	
}

// Called when the game starts or when spawned
void ADestinyFPSBase::BeginPlay()
{
	Super::BeginPlay();

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
		}
	}

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void ADestinyFPSBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurSkillCoolTime > 0.f)
		CurSkillCoolTime -= DeltaTime;

	if (CurGrenadeCoolTime > 0.f)
		CurGrenadeCoolTime -= DeltaTime;
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
	}
}

void ADestinyFPSBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

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

	UWorld* world = GetWorld();

	if(CurSkillCoolTime <= 0.f)
	{
		if(world)
		{
			FActorSpawnParameters SpawnParams;
			FVector SpawnLocation = this->GetActorLocation() + this->GetActorRotation().Vector() * 100.f;
			SpawnLocation.Z += 20.f;
			FRotator SpawnRotation = this->GetActorRotation();
			ATitan_Skill_Barrier* skillObject = GetWorld()->SpawnActor<ATitan_Skill_Barrier>(ATitan_Skill_Barrier::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

			CurSkillCoolTime = SkillCoolTime;
			isShield = true;
		}
	}
}

void ADestinyFPSBase::Grenade(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("쿨타임 : %f"), CurGrenadeCoolTime));

	UWorld* world = GetWorld(); 

	if (CurGrenadeCoolTime <= 0.f)
	{
		if (world)
		{
			FActorSpawnParameters SpawnParams;
			FVector SpawnLocation = this->GetActorLocation() + this->GetActorRotation().Vector() * 50.f;
			FRotator SpawnRotation = this->GetActorRotation();
			ATitan_Skill_Grenade* grenadeObject = GetWorld()->SpawnActor<ATitan_Skill_Grenade>(ATitan_Skill_Grenade::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
			grenadeObject->FireInDirection(this->GetActorForwardVector() + this->GetActorUpVector() / 5.f);
			CurGrenadeCoolTime = GrenadeCoolTime;
			isGrenade = true;
		}
	}
}

void ADestinyFPSBase::Ultimate(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("궁극기"));
}
void ADestinyFPSBase::jump(const FInputActionValue& Value)
{
	ACharacter::Jump();

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
