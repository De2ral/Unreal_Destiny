// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Murdock.h"
#include "AIController_Murdock.h"

ACPP_Murdock::ACPP_Murdock()
{
    PrimaryActorTick.bCanEverTick = true;

	HP = MaxHP;
    isAttack = false;
}

void ACPP_Murdock::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_Murdock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_Murdock::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
