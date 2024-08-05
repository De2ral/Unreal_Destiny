// Fill out your copyright notice in the Description page of Project Settings.


#include "DestinyFPSBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Titan_Skill_Barrier.h"


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
}

void ADestinyFPSBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SkillCoolTime > 0.f)
		SkillCoolTime -= DeltaTime;
}

void ADestinyFPSBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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

	if(SkillCoolTime <= 0.f)
	{
		if(world)
		{
			FActorSpawnParameters SpawnParams;
			FVector spawnLocation = this->GetActorLocation() + this->GetActorRotation().Vector() * 200.f;
			spawnLocation.Z -= 15.f;
			FRotator spawnRotation = this->GetActorRotation();
			ATitan_Skill_Barrier* skillObject = GetWorld()->SpawnActor<ATitan_Skill_Barrier>(ATitan_Skill_Barrier::StaticClass(), spawnLocation, spawnRotation, SpawnParams);

			SkillCoolTime = 20.f;
		}
	}
}

void ADestinyFPSBase::Grenade(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("수류탄 투척"));
}

void ADestinyFPSBase::Ultimate(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("궁극기"));
}