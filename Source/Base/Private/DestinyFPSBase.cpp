// Fill out your copyright notice in the Description page of Project Settings.


#include "DestinyFPSBase.h"
#include "Camera/CameraComponent.h"
// #include "EnhancedInputComponent.h"
// #include "EnhancedInputSubsystems.h"

// Sets default values
ADestinyFPSBase::ADestinyFPSBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetMesh(),FName("head"));
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
