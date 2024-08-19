// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Kallari.h"
#include "AIController_Kallari.h"

ACPP_Kallari::ACPP_Kallari()
{
    PrimaryActorTick.bCanEverTick = true;

	HP = MaxHP;
    isAttack = false;
}

void ACPP_Kallari::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_Kallari::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_Kallari::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
