// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Boss_ConsecratedMind.h"
#include "AIController_Boss.h"

ACPP_Boss_ConsecratedMind::ACPP_Boss_ConsecratedMind()
{
	PrimaryActorTick.bCanEverTick = true;

    HP = MaxHP;
    isAttack = false;

    CoreMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMeshComponent"));
    CoreMesh->SetupAttachment(RootComponent);
}

void ACPP_Boss_ConsecratedMind::BeginPlay()
{
    Super::BeginPlay();
}

void ACPP_Boss_ConsecratedMind::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACPP_Boss_ConsecratedMind::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
