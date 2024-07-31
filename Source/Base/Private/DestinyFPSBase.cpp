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


// Sets default values
ADestinyFPSBase::ADestinyFPSBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FppMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPPMesh"));
	TppMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TPPMesh"));

	TppMesh->SetVisibility(false);

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

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if(Subsystem != nullptr)
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}


}

void ADestinyFPSBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (Input != nullptr)
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADestinyFPSBase::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADestinyFPSBase::Look);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ADestinyFPSBase::jump);
		Input->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ADestinyFPSBase::Sprint);
		Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &ADestinyFPSBase::SprintEnd);
		Input->BindAction(SlideAction, ETriggerEvent::Triggered, this, &ADestinyFPSBase::Slide);
		Input->BindAction(SlideAction, ETriggerEvent::Completed, this, &ADestinyFPSBase::SlideEnd);
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

void ADestinyFPSBase::jump(const FInputActionValue& Value)
{
	ACharacter::Jump();

}

void ADestinyFPSBase::Sprint(const FInputActionValue& Value)
{
	bPlayerSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;

}

void ADestinyFPSBase::SprintEnd(const FInputActionValue& Value)
{
	bPlayerSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;

}

void ADestinyFPSBase::Slide(const FInputActionValue& Value)
{
	GetCharacterMovement()->Crouch(true);
	GetCharacterMovement()->MaxWalkSpeedCrouched = 2500.0f;

}

void ADestinyFPSBase::SlideEnd(const FInputActionValue& Value)
{
	GetCharacterMovement()->Crouch(false);
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;

}