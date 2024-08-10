
#include "CPP_ItemBase.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ItemComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACPP_ItemBase::ACPP_ItemBase()
{ 	
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root; 
    Root->SetMobility(EComponentMobility::Static);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(Root);
    Mesh->SetSimulatePhysics(true);
    
    // PickupItem 컴포넌트를 동적으로 추가
    Item = CreateDefaultSubobject<UItemComponent>(TEXT("ItemComponent"));    
    
    Item->ItemCollider->SetupAttachment(Mesh);  
    // Item->ItemCollider->RegisterComponent();  
}

void ACPP_ItemBase::BeginPlay()
{
	Super::BeginPlay();
	
	// 아이템 액터에 랜덤한 힘을 가해주기
	FVector RandomDirection = UKismetMathLibrary::RandomUnitVector();
    RandomDirection.Z = FMath::FRandRange(0.5f, 1.0f); // 아이템이 위쪽으로 약간 밀리도록 설정

    float ImpulseStrength = FMath::FRandRange(300.0f, 500.0f); // 힘 세기 랜덤 설정
    FVector Impulse = RandomDirection * ImpulseStrength;

    Mesh->AddImpulse(Impulse, NAME_None, true);

    if (Item && Item->ItemCollider && Mesh)
    {
        // 아이템 콜리더를 메쉬에 부착
        Item->ItemCollider->SetupAttachment(Mesh);        
    }

}

void ACPP_ItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);   

}

