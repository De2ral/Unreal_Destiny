// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Mage.h"
#include "AIController_Mage.h"



ACPP_Mage::ACPP_Mage()
{
	PrimaryActorTick.bCanEverTick = true;

	HP = MaxHP;
    isAttack = false;
    
}

void ACPP_Mage::BeginPlay()
{
	Super::BeginPlay();
		
}

void ACPP_Mage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ACPP_Mage::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACPP_Mage::PerformLineTrace()
{
	FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    StartPoint = GetActorLocation();
    EndPoint = StartPoint + FVector(GetActorForwardVector() * 400);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartPoint,
        EndPoint,
        ECC_Visibility,
        Params
    );

    DrawDebugLine(
        GetWorld(),
        StartPoint,
        EndPoint,
        bHit ? FColor::Red : FColor::Green,
        false,
        1,
        0,
        1
    );
}
