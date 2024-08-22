// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BossWall.h"

// Sets default values
ACPP_BossWall::ACPP_BossWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
    bAlwaysRelevant = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Collider->SetupAttachment(RootComponent);	
	Mesh->SetupAttachment(Collider);

	//MoveDirection = FVector(1, 0, 0);
    MoveSpeed = 100.0f;

	  // 포워드 벡터를 가져온다

    // 이동 거리를 정의한다 (예: 100 유닛)
    //MoveSpeed = 100.0f;

    // 액터를 포워드 벡터 방향으로 이동시킨다
    // FVector NewLocation = GetActorLocation() + (ForwardVector * MoveDistance);
    // SetActorLocation(NewLocation);
    
    // // 또는, 물리적 힘을 적용하여 이동할 수도 있다
    // if (UStaticMeshComponent* MeshComp = GetStaticMeshComponent())
    // {
        //Mesh->AddForce(ForwardVector * MoveSpeed); // ForceAmount는 적용할 힘
    //}

}

// Called when the game starts or when spawned
void ACPP_BossWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_BossWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FVector NewLocation = GetActorLocation() + (MoveDirection * MoveSpeed * DeltaTime);
    //SetActorLocation(NewLocation);
    FVector ForwardVector = GetActorForwardVector();
	FVector NewLocation = GetActorLocation() + (ForwardVector * MoveSpeed);
    SetActorLocation(NewLocation);
	
}

void ACPP_BossWall::MoveWall()
{
	PrimaryActorTick.bCanEverTick = true;
    MoveDirection = GetActorForwardVector();
	//GetActorForwardVector();
    MoveSpeed = 100.0f;
}

