// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Dorongo.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "AIController_Dorongo.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACPP_Dorongo::ACPP_Dorongo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HP = MaxHP;
}

// Called when the game starts or when spawned
void ACPP_Dorongo::BeginPlay()
{
	Super::BeginPlay();
		
}

// Called every frame
void ACPP_Dorongo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	 PerformLineTrace();

}

// Called to bind functionality to input
void ACPP_Dorongo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACPP_Dorongo::PerformLineTrace()
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
    
    // if (bHit)
    // {
    //     GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hit: %s"), *HitResult.GetActor()->GetName()));
    // }

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

void ACPP_Dorongo::TakeDamage1(float DamageAmount)
{
    HP -= DamageAmount;

    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HP : %d"), HP));

    if(HP <= 0.0f && !isDead)
    {
        //PhysicsMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PhysicsMesh"));
        //RootComponent = PhysicsMesh;

        // Set the mesh component to simulate physics
        //PhysicsMesh->SetSimulatePhysics(true);
        //Destroy();
        isDead = true;
        ItemDrop();
        EnablePhysicsSimulation();              
    }
}

float ACPP_Dorongo::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
    HP -= DamageAmount;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TakeDamage")));

    if(HP <= 0.0f && !isDead)
    {
        isDead = true;
        ItemDrop();
        EnablePhysicsSimulation();              
    }
    return DamageAmount;
}

void ACPP_Dorongo::EnablePhysicsSimulation()
{    
     // 사망시 AI Controller에 정보 전달. 후 블랙보드에 값을 입력하여, 처리하는건 AIContorller의 역할
        AAIController_Dorongo* AIController = Cast<AAIController_Dorongo>(GetController());

        if(AIController)
        {
            AIController->SetBlackboardValue("IsDead",true);
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("캐스팅 실패")));
        }
        
    // 캡슐 컴포넌트를 비활성화하고 피직스 시뮬레이션 활성화
    //GetCapsuleComponent()->SetSimulatePhysics(true);
    //GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //GetCharacterMovement()->DisableMovement();

    //충돌 방향 설정 (바라보는 방향 뒤로)
	FVector ImpulseDirection = GetActorRotation().Vector() * -1.0f;
	ImpulseDirection.Normalize();

	//충돌 세기
	float ImpulseStrength = 5000.0f;
    FVector FinalImpulse = ImpulseDirection * ImpulseStrength;
    this->GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
	this->GetMesh()->AddImpulseToAllBodiesBelow(FinalImpulse, NAME_None);	
    //this->GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
    //this->GetMesh()->SetSimulatePhysics(true);
    
}

void ACPP_Dorongo::ItemDrop()
{
    // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("들어오기는 했는데..")));

    // if (ItemToSpawn)
    // {
    //     FVector Location = GetActorLocation();
    //     FRotator Rotation = GetActorRotation();
    //     GetWorld()->SpawnActor<AActor>(ItemToSpawn, Location, Rotation);
    // }
    if (ItemsToSpawn.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, ItemsToSpawn.Num() - 1);
        TSubclassOf<AActor> SelectedItem = ItemsToSpawn[RandomIndex];

        if (SelectedItem)
        {
            FVector Location = GetActorLocation();
            FRotator Rotation = GetActorRotation();
            GetWorld()->SpawnActor<AActor>(SelectedItem, Location, Rotation);
        }
    }
}
