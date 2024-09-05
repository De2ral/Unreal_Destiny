// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Galhak.h"
#include "AIController_Galhak.h"



ACPP_Galhak::ACPP_Galhak()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HP = MaxHP;
    isAttack = false;
    
}

// Called when the game starts or when spawned
void ACPP_Galhak::BeginPlay()
{
	Super::BeginPlay();
		
}

// Called every frame
void ACPP_Galhak::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//PerformLineTrace();

}

// Called to bind functionality to input
void ACPP_Galhak::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACPP_Galhak::PerformLineTrace()
{
	FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // 자기 자신을 무시

    // 라인 트레이스를 수행할 지점 설정 Tick마다 위치 갱신
    StartPoint = GetActorLocation();
    EndPoint = StartPoint + FVector(GetActorForwardVector() * 400); // 앞으로 라인트레이스 생성

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartPoint,
        EndPoint,
        ECC_Visibility,
        Params
    );

    // 디버그 라인 그리기
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
