// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Dendron.h"
#include "AIController_Dendron.h"

ACPP_Dendron::ACPP_Dendron()
{
    PrimaryActorTick.bCanEverTick = true;

	HP = MaxHP;
    isAttack = false;
}
void ACPP_Dendron::BeginPlay()
{
    Super::BeginPlay();
}
void ACPP_Dendron::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsLineTraceActive)
    {
        PerformLineTrace();
    }
    // else
    // {
    //     LastPoint = EndPoint;
    //     LastPerformLineTrace();
    // }
}
void ACPP_Dendron::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACPP_Dendron::PerformLineTrace()
{
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // 자기 자신을 무시
    APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

    if (PlayerPawn && !LastTargetPosition)
    {
        EndPoint = PlayerPawn->GetActorLocation();
    }
    else
    {        
        StartPoint + FVector(GetActorForwardVector() * 400);
        LastPoint = EndPoint;
    }
    
    StartPoint = GetActorLocation();
    //EndPoint = StartPoint + FVector(GetActorForwardVector() * 400);
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartPoint,
        EndPoint,
        ECC_Visibility,
        Params
    );
    
    
    // 디버그 라인 그리기
    if(!LastTargetPosition)
    {
    DrawDebugLine(
        GetWorld(),
        StartPoint,
        EndPoint,
        bHit ? FColor::Red : FColor::Red,
        false,
        0.03f,
        0,
        1
    );
    }
    else
    {
          DrawDebugLine(
        GetWorld(),
        StartPoint,
        LastPoint,
        bHit ? FColor::Red : FColor::Red,
        false,
        0.03f,
        0,
        1
    );
    }
}

void ACPP_Dendron::LastPerformLineTrace()
{
     FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // 자기 자신을 무시
    APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

    if (PlayerPawn)
    {
        EndPoint = PlayerPawn->GetActorLocation();
    }
    else
    {        
        StartPoint + FVector(GetActorForwardVector() * 400);
    }
    
    StartPoint = GetActorLocation();
    //EndPoint = StartPoint + FVector(GetActorForwardVector() * 400);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartPoint,
        LastPoint,
        ECC_Visibility,
        Params
    );
    
    // 디버그 라인 그리기
    DrawDebugLine(
        GetWorld(),
        StartPoint,
        LastPoint,
        bHit ? FColor::Red : FColor::Red,
        false,
        0.03f,
        0,
        1
    );
}
