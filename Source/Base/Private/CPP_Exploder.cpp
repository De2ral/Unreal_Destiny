// Fill out your copyright notice in the Description page of Project Settings.



#include "CPP_Exploder.h"
#include "AIController_Exploder.h"

ACPP_Exploder::ACPP_Exploder()
{
    PrimaryActorTick.bCanEverTick = true;

	HP = MaxHP;
    isAttack = false;
}

void ACPP_Exploder::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_Exploder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_Exploder::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

