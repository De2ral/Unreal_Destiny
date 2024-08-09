// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "DestinyFPSBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "FpsCppProjectile.h"

#include "Camera/CameraShakeBase.h"
#include "MyLegacyCameraShake.h"

#include "Kismet/GameplayStatics.h"

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    bIsAiming = false;

    AimingSpeed = 5.0f; 
    
    AimOffset = FVector(15.0f, 0.0f, 0.0f);
    AimRotation = FRotator(10.0f, -15.0f, 0.0f);
}



void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    ADestinyFPSBase* PlayerCharacter = Cast<ADestinyFPSBase>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (PlayerCharacter != nullptr)
    {
        EquipWeapon3();
    }
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter called failed."));
	}

    AddMapping(PlayerCharacter);

    if (AmmoWidgetClass)
	{
        CurrentAmmo = MaxAmmo;
		AmmoWidget = CreateWidget<UTestWidget>(GetWorld(), AmmoWidgetClass);
		if (AmmoWidget)
		{
			AmmoWidget->AddToViewport();
			AmmoWidget->UpdateAmmo(CurrentAmmo, MaxAmmo);
		}
	}
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AmmoWidgetClass called failed."));
    }
}



void UWeaponComponent::Fire()
{
    UE_LOG(LogTemp, Warning, TEXT("Fire2"));

    CurrentAmmo--;
    AmmoWidget->UpdateAmmo(CurrentAmmo, MaxAmmo);
    
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());


            if (PlayerController == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("PlayerController is null."));
            return;
        }


			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			//const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
            //FVector MuzzleLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
            FVector MuzzleLocation;
            if(CurrentSkeletalMeshComponent)
            {
                MuzzleLocation = CurrentSkeletalMeshComponent->GetComponentLocation() + (CurrentSkeletalMeshComponent->GetRightVector() * 100.0f);
                UE_LOG(LogTemp, Warning, TEXT("MyVector Location: %s"), *CurrentSkeletalMeshComponent->GetComponentLocation().ToString()); 
                UE_LOG(LogTemp, Warning, TEXT("MyVector forward: %s"), *CurrentSkeletalMeshComponent->GetForwardVector().ToString()); 
                UE_LOG(LogTemp, Warning, TEXT("MyVector MuzzleLocation: %s"), *MuzzleLocation.ToString()); 
            }
            
            


			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			AFpsCppProjectile* Projectile = World->SpawnActor<AFpsCppProjectile>(ProjectileClass, MuzzleLocation, SpawnRotation, ActorSpawnParams);
			
            if (Projectile == nullptr)
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to spawn projectile."));
                return;
            }
            
			const FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
			
			// 라인트레이스 시작점과 끝점 설정
			const FVector TraceStart = CameraLocation;
			const FVector TraceEnd = TraceStart + (SpawnRotation.Vector() * 1000000.0f);

			FHitResult HitResult;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(Character);
			CollisionParams.AddIgnoredActor(Projectile);
			
			// 라인트레이스 수행
			bool bHit = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);

			// 디버그 라인 그리기
			DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 1, 0, 1);

			FVector ProjectileDirection = SpawnRotation.Vector();
			
			if (bHit)
			{
				// 라인트레이스가 맞은 위치 표시
				DrawDebugSphere(World, HitResult.Location, 10.0f, 12, FColor::Green, false, 1.0f);

				UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
				UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *HitResult.Location.ToString());
				
				ProjectileDirection = (HitResult.Location - MuzzleLocation).GetSafeNormal();			
			}

            if (Projectile->GetProjectileMovement())
            {
                Projectile->GetProjectileMovement()->Velocity = ProjectileDirection * Projectile->GetProjectileMovement()->InitialSpeed;
            }


			if (PlayerController != nullptr)
    		{
                if(bIsAiming)
        		    PlayerController->ClientStartCameraShake(UMyLegacyCameraShake::StaticClass(), CurrentWeapon.Rebound * 0.5f);
                else
                    PlayerController->ClientStartCameraShake(UMyLegacyCameraShake::StaticClass(), CurrentWeapon.Rebound);
   			}
		} 
}

void UWeaponComponent::EquipWeapon1()
{
	UE_LOG(LogTemp, Warning, TEXT("1"));
    LoadWeaponByName(FName("Pistol"));
    FString ModelPath = CurrentWeapon.GunModelPath;
    LoadAndAttachModelToCharacter(Cast<ADestinyFPSBase>(UGameplayStatics::GetPlayerCharacter(this, 0)), ModelPath);
}

void UWeaponComponent::EquipWeapon2()
{
	UE_LOG(LogTemp, Warning, TEXT("2"));
    LoadWeaponByName(FName("Pistol2"));
    FString ModelPath = CurrentWeapon.GunModelPath;
    LoadAndAttachModelToCharacter(Cast<ADestinyFPSBase>(UGameplayStatics::GetPlayerCharacter(this, 0)), ModelPath);
}

void UWeaponComponent::EquipWeapon3()
{
	UE_LOG(LogTemp, Warning, TEXT("3"));
    LoadWeaponByName(FName("Pistol3"));
    FString ModelPath = CurrentWeapon.GunModelPath;
    LoadAndAttachModelToCharacter(Cast<ADestinyFPSBase>(UGameplayStatics::GetPlayerCharacter(this, 0)), ModelPath);
}

void UWeaponComponent::AddMapping(ADestinyFPSBase* TargetCharacter)
{
    Character = TargetCharacter;
    if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(FireMappingContext, 1);
        }

        if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
        {
            EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UWeaponComponent::Fire);
            EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &UWeaponComponent::StartAiming);
            EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &UWeaponComponent::StopAiming);
            EnhancedInputComponent->BindAction(Equip1Action, ETriggerEvent::Started, this, &UWeaponComponent::EquipWeapon1);
            EnhancedInputComponent->BindAction(Equip2Action, ETriggerEvent::Started, this, &UWeaponComponent::EquipWeapon2);
            EnhancedInputComponent->BindAction(Equip3Action, ETriggerEvent::Started, this, &UWeaponComponent::EquipWeapon3);
        }
    }
}

void UWeaponComponent::AttachModelToCharacter(ADestinyFPSBase* TargetCharacter, UObject* Model)
{
    RemoveCurrentWeaponModel();
    Character = TargetCharacter;
    if (Character == nullptr || Model == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Character or Model is null."));
        return;
    }

    if (USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(Model))
    {
        CurrentSkeletalMeshComponent = NewObject<USkeletalMeshComponent>(Character);
        CurrentSkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);

        FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
        CurrentSkeletalMeshComponent->AttachToComponent(Character->GetFppMesh(), AttachmentRules, FName(TEXT("GripPoint")));
        CurrentSkeletalMeshComponent->RegisterComponent();

        UE_LOG(LogTemp, Warning, TEXT("SkeletalMesh"));

        DefaultOffset =  CurrentSkeletalMeshComponent->GetRelativeLocation();
        DefaultRotation = CurrentSkeletalMeshComponent->GetRelativeRotation();
    }
    else if (UStaticMesh* StaticMesh = Cast<UStaticMesh>(Model))
    {
        CurrentStaticMeshComponent = NewObject<UStaticMeshComponent>(Character);
        CurrentStaticMeshComponent->SetStaticMesh(StaticMesh);
        FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
        CurrentStaticMeshComponent->AttachToComponent(Character->GetFppMesh(), AttachmentRules, FName(TEXT("GripPoint")));
        CurrentStaticMeshComponent->RegisterComponent();

        UE_LOG(LogTemp, Warning, TEXT("StaticMesh"));

        DefaultOffset =  CurrentStaticMeshComponent->GetRelativeLocation();
        DefaultRotation = CurrentStaticMeshComponent->GetRelativeRotation();
    }

    Character->SetHasRifle(true);
    
}



void UWeaponComponent::AttachSelectedModelToCharacter(ADestinyFPSBase *InCharacter, UObject *SelectedModel)
{
    if (InCharacter == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Character is null."));
        return;
    }

	else if (SelectedModel == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedModel is null."));
        return;
	}
	

    UWeaponComponent* WeaponComponent = InCharacter->FindComponentByClass<UWeaponComponent>();
    if (WeaponComponent == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("WeaponComponent not found on Character."));
        return;
    }

    WeaponComponent->AttachModelToCharacter(InCharacter, SelectedModel);
}

void UWeaponComponent::LoadAndAttachModelToCharacter(ADestinyFPSBase *InCharacter, const FString &ModelPath)
{
    if (InCharacter == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Character is null."));
        return;
    }

    UObject* LoadedModel = StaticLoadObject(UObject::StaticClass(), nullptr, *ModelPath);
    if (LoadedModel == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load model from path: %s"), *ModelPath);
        return;
    }

    AttachSelectedModelToCharacter(InCharacter, LoadedModel);
}

void UWeaponComponent::SetCurrentWeapon(const FGunInfo& NewWeapon)
{
    CurrentWeapon = NewWeapon;
}


void UWeaponComponent::LoadWeaponByName(FName WeaponName)
{
    if (WeaponDataTable)
    {
        FGunInfo* WeaponData = WeaponDataTable->FindRow<FGunInfo>(WeaponName, TEXT("Weapon Lookup"));
        if (WeaponData)
        {
            SetCurrentWeapon(*WeaponData);
            UE_LOG(LogTemp, Warning, TEXT("Loaded Weapon: %s"), *WeaponName.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Weapon not found: %s"), *WeaponName.ToString());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("WeaponDataTable is null."));
    }
}

void UWeaponComponent::RemoveCurrentWeaponModel()
{
    if (CurrentStaticMeshComponent)
    {
        CurrentStaticMeshComponent->DestroyComponent();
        CurrentStaticMeshComponent = nullptr;
    }

    if (CurrentSkeletalMeshComponent)
    {
        CurrentSkeletalMeshComponent->DestroyComponent();
        CurrentSkeletalMeshComponent = nullptr;
    }
}

void UWeaponComponent::StartAiming()
{
    bIsAiming = true;
}

void UWeaponComponent::StopAiming()
{
   bIsAiming = false;
}

void UWeaponComponent::UpdateWeaponPosition()
{
    if (Character == nullptr)
        return;

    FVector TargetOffset = bIsAiming ? AimOffset : DefaultOffset;
    FRotator TargetRotation = bIsAiming ? AimRotation : DefaultRotation;

    if (CurrentStaticMeshComponent)
    {
        CurrentStaticMeshComponent->SetRelativeLocation(TargetOffset);
        CurrentStaticMeshComponent->SetRelativeRotation(TargetRotation);
    }

    if (CurrentSkeletalMeshComponent)
    {
        CurrentSkeletalMeshComponent->SetRelativeLocation(TargetOffset);
        CurrentSkeletalMeshComponent->SetRelativeRotation(TargetRotation);

        
    }
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (CurrentSkeletalMeshComponent)
    {
        FVector TargetLocation = bIsAiming ? AimOffset : DefaultOffset;
        FRotator TargetRotation = bIsAiming ? AimRotation : DefaultRotation;

        FVector NewLocation = FMath::VInterpTo(CurrentSkeletalMeshComponent->GetRelativeLocation(), TargetLocation, DeltaTime, AimingSpeed);
        FRotator NewRotation = FMath::RInterpTo(CurrentSkeletalMeshComponent->GetRelativeRotation(), TargetRotation, DeltaTime, AimingSpeed);

        CurrentSkeletalMeshComponent->SetRelativeLocation(NewLocation);
        CurrentSkeletalMeshComponent->SetRelativeRotation(NewRotation);
    }
}

void UWeaponComponent::UpdateAmmoDisplay()
{
}