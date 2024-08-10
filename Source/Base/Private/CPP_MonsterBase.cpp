// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_MonsterBase.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACPP_MonsterBase::ACPP_MonsterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_MonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_MonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPP_MonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACPP_MonsterBase::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{	
    if(!bCanTakeDamage) return 0.0f;

	float Damage = Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator, DamageCauser);
    
    HP -= DamageAmount;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TakeDamage")));

    if(HP <= 0.0f && !isDead)
    {
        isDead = true;
		int32 ItemCount = FMath::RandRange(MinItemValue, MaxItemValue);	
        ItemDrop(ItemCount);
        EnablePhysicsSimulation();              
    }
    FTimerHandle UnusedHandle; 
    GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &ACPP_MonsterBase::ResetDamageCoolDown, DamageCooldownTime, false);

    return Damage;
    
}

void ACPP_MonsterBase::ResetDamageCoolDown()
{
	bCanTakeDamage = true;
}

void ACPP_MonsterBase::EnablePhysicsSimulation()
{
	 // 사망시 AI Controller에 정보 전달. 후 블랙보드에 값을 입력하여, 처리하는건 AIContorller의 역할
        //AAIController_Dorongo* AIController = Cast<AAIController_Dorongo>(GetController());

	// 부모 클래스가 생성됨에 따라, 각 Owner가 소유한 Controller에 접근시켜주도록 변경.
	AController* MController = GetController();
	if(MController)
	{
		AAIController* AIController = Cast<AAIController>(MController);		
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		if(BlackboardComp)
		{
			BlackboardComp->SetValueAsBool(FName("IsDead"),true);
		}
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("캐스팅 실패")));
        }
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

void ACPP_MonsterBase::ItemDrop(int32 ItemCount)
{
	//if(!ItemsToSpawn) return;

	for(int32 i=0;i<ItemCount;++i)
	{
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
	
}
