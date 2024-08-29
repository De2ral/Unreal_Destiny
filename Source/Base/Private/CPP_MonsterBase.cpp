// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_MonsterBase.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SphereComponent.h"
#include "HitDamageEvent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACPP_MonsterBase::ACPP_MonsterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	
    bReplicates = true; 

	Tags.Add(FName("Enemy"));
    if(GetMesh())
    {
        GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
    }
    else
    {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("캐스팅 실패")));

    }

    CriticalHitBox = CreateDefaultSubobject<USphereComponent>(TEXT("CriticalHitBox"));
    CriticalHitBox->SetupAttachment(RootComponent);
    
    CriticalHitBox->SetCollisionProfileName(TEXT("Trigger"));
    CriticalHitBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_MonsterBase::OnCriticalHitBoxOverlap);

    bIsCriticalHit = false;


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

void ACPP_MonsterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
     Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // HP 변수를 리플리케이트하도록 설정
    DOREPLIFETIME(ACPP_MonsterBase, HP);    
}

void ACPP_MonsterBase::OnRep_HP()
{
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TakeDamage OnRep")));

}

float ACPP_MonsterBase::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{	
    //if(!bCanTakeDamage) return 0.0f;

    if (HasAuthority())
    {
        // 서버에서 HP 감소
        ServerTakeDamage(DamageAmount);
    }
    else
    {
        // 클라이언트에서 서버로 HP 감소 요청
        ServerTakeDamage(DamageAmount);
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Client is requesting,Damage Amount : %f HP : %f"),DamageAmount,HP));

    }
    
	//float Damage = Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator, DamageCauser);
    //return Damage;
     return Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator, DamageCauser);
    
}

void ACPP_MonsterBase::MulticastTakeDamage_Implementation(float DamageAmount)
{
      if(bIsCriticalHit)
    {
        DamageAmount *= 2.0f;
        bIsCriticalTextColor = true;
    }
    //if(HasAuthority())
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ServerTakeDamage in - Server")));
    //else
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ServerTakeDamage in - Client")));

    // 서버에서만 HP를 감소시킴
    HP -= DamageAmount;

    DamageValue = DamageAmount;
    
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Server HP decreased by %f. New HP: %f"), DamageAmount, HP));

    if(HP <= 0.0f && !isDead)
    {
        isDead = true;
		int32 ItemCount = FMath::RandRange(MinItemValue, MaxItemValue);	
        ItemDrop(ItemCount);
        EnablePhysicsSimulation();              
        GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ACPP_MonsterBase::DestroyActor, 5.0f, false);
    }
    FTimerHandle UnusedHandle; 
    //GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &ACPP_MonsterBase::ResetDamageCoolDown, DamageCooldownTime, false);
    
    bIsCriticalHit = false;
}

void ACPP_MonsterBase::ServerTakeDamage_Implementation(float DamageAmount)
{
     if(bIsCriticalHit)
    {
        DamageAmount *= 2.0f;
        bIsCriticalTextColor = true;
    }
    //if(HasAuthority())
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ServerTakeDamage in - Server")));
    //else
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ServerTakeDamage in - Client")));

    // 서버에서만 HP를 감소시킴
    HP -= DamageAmount;

    DamageValue = DamageAmount;
    
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Server HP decreased by %f. New HP: %f"), DamageAmount, HP));

    if(HP <= 0.0f && !isDead)
    {
        isDead = true;
		int32 ItemCount = FMath::RandRange(MinItemValue, MaxItemValue);	
        ItemDrop(ItemCount);
        EnablePhysicsSimulation();              
        GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ACPP_MonsterBase::DestroyActor, 5.0f, false);
    }
    FTimerHandle UnusedHandle; 
    //GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &ACPP_MonsterBase::ResetDamageCoolDown, DamageCooldownTime, false);
    
    bIsCriticalHit = false;
    // 변경된 HP 값이 자동으로 클라이언트에 리플리케이트됨
    //MulticastTakeDamage(DamageAmount);
}



bool ACPP_MonsterBase::ServerTakeDamage_Validate(float DamageAmount)
{
    // 유효성 검사 (필요 시 구현)
    return true;
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

void ACPP_MonsterBase::OnCriticalHitBoxOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if (OtherActor && OtherActor != this)
    {        
        bIsCriticalHit = true;
    }
}

void ACPP_MonsterBase::DestroyActor()
{
    Destroy();
}
