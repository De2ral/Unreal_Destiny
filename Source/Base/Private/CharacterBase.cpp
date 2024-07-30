// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

