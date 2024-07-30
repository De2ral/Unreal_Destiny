// Fill out your copyright notice in the Description page of Project Settings.


#include "DestinyFPSBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
// #include "EnhancedInputComponent.h"
// #include "EnhancedInputSubsystems.h"

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

	FppCamera->SetupAttachment(FppMesh);
	TppCamera->SetupAttachment(TppSpringArm);

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
	

}

void ADestinyFPSBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForwardBackward"),this,&ADestinyFPSBase::MoveForwardBackward);
	PlayerInputComponent->BindAxis(FName("MoveLeftRight"),this,&ADestinyFPSBase::MoveLeftRight);
	PlayerInputComponent->BindAxis(FName("LookUp"),this,&ADestinyFPSBase::LookUp);
	PlayerInputComponent->BindAxis(FName("LookRight"),this,&ADestinyFPSBase::LookRight);
}

void ADestinyFPSBase::MoveForwardBackward(const float Value)
{
	const FVector& Direction = FRotationMatrix(FRotator(0.f,GetControlRotation().Yaw,0.f)).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ADestinyFPSBase::MoveLeftRight(const float Value)
{
	const FVector& Direction = FRotationMatrix(FRotator(0.f,GetControlRotation().Yaw,0.f)).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);

}

void ADestinyFPSBase::LookUp(const float Value)
{
	AddControllerPitchInput(Value);

}

void ADestinyFPSBase::LookRight(const float Value)
{
	AddControllerYawInput(Value);
}
