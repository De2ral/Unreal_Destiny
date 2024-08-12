// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"

// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	AActor* Parent = GetOwner();

	ItemCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ItemCollider"));
	ItemCollider->InitSphereRadius(9.0f);
	ItemCollider->SetMobility(EComponentMobility::Movable);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetWorldScale3D(FVector3d(3.0f,3.0f,3.0f));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/ParagonDrongo/FX/Meshes/Heroes/Drongo/SM_Drongo_Grenade_FX_Body02.SM_Drongo_Grenade_FX_Body02'"));
	ItemMesh->SetStaticMesh(MeshAsset.Object);
	ItemMesh->SetSimulatePhysics(true);

	SpecAmmoMaterial = CreateDefaultSubobject<UMaterial>(TEXT("SpecAmmoMat"));
	static ConstructorHelpers::FObjectFinder<UMaterial>FoundMaterial(TEXT("/Script/Engine.Material'/Game/DestinyFPS/Items/SpecAmmoMat.SpecAmmoMat'"));
	SpecAmmoMaterial = FoundMaterial.Object;

	NormalAmmoMaterial = CreateDefaultSubobject<UMaterial>(TEXT("NormAmmoMat"));
	static ConstructorHelpers::FObjectFinder<UMaterial>FoundMaterial2(TEXT("/Script/Engine.Material'/Game/DestinyFPS/Items/NormalAmmoMat.NormalAmmoMat'"));
	NormalAmmoMaterial = FoundMaterial2.Object;

	RefAmmoMaterial = CreateDefaultSubobject<UMaterial>(TEXT("RefAmmoMat"));
	static ConstructorHelpers::FObjectFinder<UMaterial>FoundMaterial3(TEXT("/Script/Engine.Material'/Game/DestinyFPS/Items/RefAmmoMat.RefAmmoMat'"));
	RefAmmoMaterial = FoundMaterial3.Object;

	if(IsValid(Parent)) 
	{
		ItemMesh->SetupAttachment(Parent->GetRootComponent());
		ItemCollider->SetupAttachment(ItemMesh);
	}

	
	// ...
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ItemCollider->OnComponentBeginOverlap.AddDynamic(this,&UItemComponent::OnOverlapBegin);

	switch (ThisItemType)
	{
	case EItemType::Ammo:
		ItemMesh->SetMaterial(0,NormalAmmoMaterial);
		break;
	case EItemType::SpecAmmo:
		ItemMesh->SetMaterial(0,SpecAmmoMaterial);
		break;
	case EItemType::RefAmmo:
		ItemMesh->SetMaterial(0,RefAmmoMaterial);
		break;
	
	default:
		break;
	}

	
}

void UItemComponent::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

	AActor* Parent = GetOwner();

	if(OtherActor->ActorHasTag(TEXT("DestinyPlayer")) && Parent)
	{
		UInventorySystem* PlayerInventory = Cast<UInventorySystem>(OtherActor->GetComponentByClass(UInventorySystem::StaticClass()));
		//여기에 아이템 - 인벤토리간 상호작용 코드
		if(ThisItemType == EItemType::Ammo /*&& !PlayerInventory->bIsAmmoFull()*/)
		{
			PlayerInventory->AddCurrAmmo(30);
			Parent->Destroy();
			
		}

		if(ThisItemType == EItemType::RefAmmo && !PlayerInventory->bIsRefAmmoFull())
		{
			PlayerInventory->AddCurrRefAmmo(20);
			Parent->Destroy();
			
		}

		if(ThisItemType == EItemType::SpecAmmo && !PlayerInventory->bIsSpecAmmoFull())
		{
			PlayerInventory->AddCurrSpecialAmmo(5);
			Parent->Destroy();
			
		}

		// else if(ThisItemType == EItemType::Weapon)
		// {


			
		// }

	}

}

// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

