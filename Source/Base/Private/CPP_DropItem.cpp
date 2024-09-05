// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_DropItem.h"
#include "string.h"

// Sets default values
ACPP_DropItem::ACPP_DropItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ItemCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ItemCollider"));
	ItemCollider->InitSphereRadius(14.0f);
	ItemCollider->SetMobility(EComponentMobility::Movable);
	//ItemCollider->SetIsReplicated(true);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetWorldScale3D(FVector3d(3.0f,3.0f,3.0f));
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/ParagonDrongo/FX/Meshes/Heroes/Drongo/SM_Drongo_Grenade_FX_Body02.SM_Drongo_Grenade_FX_Body02'"));
	ItemMesh->SetStaticMesh(MeshAsset.Object);
	ItemMesh->SetSimulatePhysics(true);
	//ItemMesh->SetIsReplicated(true);

	SpecAmmoMaterial = CreateDefaultSubobject<UMaterial>(TEXT("SpecAmmoMat"));
	static ConstructorHelpers::FObjectFinder<UMaterial>AmmoMaterial1(TEXT("/Script/Engine.Material'/Game/DestinyFPS/Items/SpecAmmoMat.SpecAmmoMat'"));
	SpecAmmoMaterial = AmmoMaterial1.Object;

	NormalAmmoMaterial = CreateDefaultSubobject<UMaterial>(TEXT("NormAmmoMat"));
	static ConstructorHelpers::FObjectFinder<UMaterial>AmmoMaterial2(TEXT("/Script/Engine.Material'/Game/DestinyFPS/Items/NormalAmmoMat.NormalAmmoMat'"));
	NormalAmmoMaterial = AmmoMaterial2.Object;

	RefAmmoMaterial = CreateDefaultSubobject<UMaterial>(TEXT("RefAmmoMat"));
	static ConstructorHelpers::FObjectFinder<UMaterial>AmmoMaterial3(TEXT("/Script/Engine.Material'/Game/DestinyFPS/Items/RefAmmoMat.RefAmmoMat'"));
	RefAmmoMaterial = AmmoMaterial3.Object;

	WeaponItemMaterial = CreateDefaultSubobject<UMaterial>(TEXT("WeaponItemMat"));
	static ConstructorHelpers::FObjectFinder<UMaterial>WeaponItemMaterialFind(TEXT("/Script/Engine.Material'/DatasmithContent/Materials/FBXImporter/VRED/MetallicCarpaint.MetallicCarpaint'"));
	WeaponItemMaterial = WeaponItemMaterialFind.Object;

	ItemMesh->SetupAttachment(RootComponent);
	ItemCollider->SetupAttachment(ItemMesh);

}

// Called when the game starts or when spawned
void ACPP_DropItem::BeginPlay()
{
	Super::BeginPlay();

	ItemCollider->OnComponentBeginOverlap.AddDynamic(this,&ACPP_DropItem::OnOverlapBegin);
	
	uint8 randomSeed;
	randomSeed = FMath::RandRange(2,5);
	switch (randomSeed)
	{
	case 2:
		ThisItemType = EDropItemType::RefAmmo;
		break;
	case 3:
		ThisItemType = EDropItemType::Ammo;
		break;
	case 4:
		ThisItemType = EDropItemType::SpecAmmo;
		break;
	case 5:
		ThisItemType = EDropItemType::Weapon;
		break;
	default:
		break;
	}

	switch (ThisItemType)
	{
	case EDropItemType::Ammo:
		ItemMesh->SetMaterial(0,NormalAmmoMaterial);
		break;
	case EDropItemType::SpecAmmo:
		ItemMesh->SetMaterial(0,SpecAmmoMaterial);
		break;
	case EDropItemType::RefAmmo:
		ItemMesh->SetMaterial(0,RefAmmoMaterial);
		break;
	case EDropItemType::Weapon:
		ItemMesh->SetMaterial(0,WeaponItemMaterial);
		break;
	}
	
}

// Called every frame
void ACPP_DropItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_DropItem::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(OtherActor->ActorHasTag(TEXT("DestinyPlayer")))
	{
		UInventorySystem* PlayerInventory = Cast<UInventorySystem>(OtherActor->GetComponentByClass(UInventorySystem::StaticClass()));
		//여기에 아이템 - 인벤토리간 상호작용 코드
		if(ThisItemType == EDropItemType::Ammo /*&& !PlayerInventory->bIsAmmoFull()*/)
		{
			PlayerInventory->AddCurrAmmo(30);
			Destroy();
			
		}

		if(ThisItemType == EDropItemType::RefAmmo && !PlayerInventory->bIsRefAmmoFull())
		{
			PlayerInventory->AddCurrRefAmmo(20);
			Destroy();
			
		}

		if(ThisItemType == EDropItemType::SpecAmmo && !PlayerInventory->bIsSpecAmmoFull())
		{
			PlayerInventory->AddCurrSpecialAmmo(5);
			Destroy();
			
		}

		if(ThisItemType == EDropItemType::Weapon)
		{

			PlayerInventory->AddWeaponToInventory();
			Destroy();
		}

	}
}
