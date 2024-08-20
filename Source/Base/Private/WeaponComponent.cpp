// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "DestinyFPSBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "FpsCppProjectile.h"
#include "Titan_Skill_Grenade.h"

#include "Camera/CameraShakeBase.h"
#include "MyLegacyCameraShake.h"

#include "Kismet/GameplayStatics.h"

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}



void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    ADestinyFPSBase* PlayerCharacter = Cast<ADestinyFPSBase>(GetOwner());
    if (PlayerCharacter != nullptr)
    {
        EquipWeapon1();
    }
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter called failed."));
	}

    AddMapping(PlayerCharacter);

    if (AmmoWidgetClass)
	{
        CurrentAmmo = MaxAmmo;
		AmmoWidget = CreateWidget<UWeaponWidget>(GetWorld(), AmmoWidgetClass);
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
    UE_LOG(LogTemp, Warning, TEXT("Fire"));

     if(CurrentAmmo < 1)
    {
        StopFiring();
        return;
    }

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
        FVector MuzzleLocation;
        if(CurrentSkeletalMeshComponent)
        {
            MuzzleLocation = CurrentSkeletalMeshComponent->GetComponentLocation() + (CurrentSkeletalMeshComponent->GetRightVector() * 100.0f);
        }
        
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        AFpsCppProjectile* Projectile = nullptr;
        if(!CurrentWeapon.Linetracing)
        {
		    Projectile = World->SpawnActor<AFpsCppProjectile>(ProjectileClass, MuzzleLocation, SpawnRotation, ActorSpawnParams);
            Projectile->SetProjectile(CurrentWeapon.ProjectileMesh, CurrentWeapon.ProjectileSpeed, CurrentWeapon.GunDamage);
        }
        
		const FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
		
		const FVector TraceStart = CameraLocation;
		const FVector TraceEnd = TraceStart + (SpawnRotation.Vector() * 1000000.0f);
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());
        if(!CurrentWeapon.Linetracing)
		    CollisionParams.AddIgnoredActor(Projectile);
		
        FCollisionObjectQueryParams ObjectQueryParams;
        ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		//bool bHit = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);
        bool bHit = World->LineTraceSingleByObjectType(HitResult, TraceStart, TraceEnd, ObjectQueryParams, CollisionParams);
		DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 1, 0, 1);
		FVector ProjectileDirection = SpawnRotation.Vector();
		
        if (FireAnimation != nullptr && !bIsAiming)
	    {
	    	ADestinyFPSBase* PlayerCharacter = Cast<ADestinyFPSBase>(GetOwner());
	    	UAnimInstance* AnimInstance = PlayerCharacter->GetFppMesh()->GetAnimInstance();
	    	if (AnimInstance != nullptr)
	    	{
	    		AnimInstance->Montage_Play(FireAnimation, 1.f);
	    	}
	    }

        if (PlayerController != nullptr)
    	{
            if(bIsAiming)
    		    PlayerController->ClientStartCameraShake(UMyLegacyCameraShake::StaticClass(), CurrentWeapon.Rebound * 0.5f);
            else
                PlayerController->ClientStartCameraShake(UMyLegacyCameraShake::StaticClass(), CurrentWeapon.Rebound);
   		}

		if (bHit)
		{
            if (HitResult.GetActor()->ActorHasTag("Enemy"))
            {
                DrawDebugSphere(World, HitResult.Location, 10.0f, 12, FColor::Green, false, 1.0f);
			    UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
			    UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *HitResult.Location.ToString());
                if(CurrentWeapon.Linetracing)
                {
                    UE_LOG(LogTemp, Warning, TEXT("linetracing"));
                    UE_LOG(LogTemp, Warning, TEXT("Damage : %f"),CurrentWeapon.GunDamage);
                    UGameplayStatics::ApplyDamage(HitResult.GetActor(), CurrentWeapon.GunDamage, PlayerController, GetOwner(), UDamageType::StaticClass());
                    return;
                }
                
            }
			ProjectileDirection = (HitResult.Location - MuzzleLocation).GetSafeNormal();			
		}
        if (Projectile)
        {
            Projectile->GetProjectileMovement()->Velocity = ProjectileDirection * Projectile->GetProjectileMovement()->InitialSpeed;
        }
       
	}
}


void UWeaponComponent::FireInRange()
{
     UE_LOG(LogTemp, Warning, TEXT("FireInRange"));

    UWorld* const World = GetWorld();
    if (World != nullptr)
    {
        APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
        
        const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
        FVector MuzzleLocation;
        if (CurrentSkeletalMeshComponent)
        {
            MuzzleLocation = CurrentSkeletalMeshComponent->GetComponentLocation() + (CurrentSkeletalMeshComponent->GetRightVector() * 100.0f);
        }

        const FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
        const FVector TraceStart = CameraLocation;
        
        const int32 NumPellets = 10; // 총알의 개수
        const float SpreadAngle = 5.0f; // 발사 범위 각도

        for (int32 i = 0; i < NumPellets; ++i)
        {
            if (CurrentAmmo < 1)
            {
                StopFiring();
                return;
            }


            CurrentAmmo--;
            AmmoWidget->UpdateAmmo(CurrentAmmo, MaxAmmo);
            // 발사 방향을 랜덤하게 조정 (앞 방향 기준)
            float RandomHorizontalSpread = FMath::RandRange(-SpreadAngle, SpreadAngle);
            float RandomVerticalSpread = FMath::RandRange(-SpreadAngle, SpreadAngle);

            // 발사 방향을 계산
            FRotator PelletRotation = SpawnRotation;
            PelletRotation.Yaw += RandomHorizontalSpread;   // 가로 스프레드
            PelletRotation.Pitch += RandomVerticalSpread;   // 세로 스프레드

            FVector ShotDirection = PelletRotation.Vector();
            FVector PelletEnd = TraceStart + (ShotDirection * 1000000.0f);

            FHitResult HitResult;
            FCollisionQueryParams CollisionParams;
            CollisionParams.AddIgnoredActor(GetOwner());

            FCollisionObjectQueryParams ObjectQueryParams;
            ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
            bool bHit = World->LineTraceSingleByObjectType(HitResult, TraceStart, PelletEnd, ObjectQueryParams, CollisionParams);
            DrawDebugLine(World, TraceStart, PelletEnd, FColor::Red, false, 1, 0, 1);

            if (bHit)
            {
                if (HitResult.GetActor()->ActorHasTag("Enemy"))
                {
                    DrawDebugSphere(World, HitResult.Location, 10.0f, 12, FColor::Green, false, 1.0f);
                    UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
                    UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *HitResult.Location.ToString());

                    UGameplayStatics::ApplyDamage(HitResult.GetActor(), CurrentWeapon.GunDamage, PlayerController, GetOwner(), UDamageType::StaticClass());
                }
            }
        }

        if (FireAnimation != nullptr && !bIsAiming)
	    {
	    	ADestinyFPSBase* PlayerCharacter = Cast<ADestinyFPSBase>(GetOwner());
	    	UAnimInstance* AnimInstance = PlayerCharacter->GetFppMesh()->GetAnimInstance();
	    	if (AnimInstance != nullptr)
	    	{
	    		AnimInstance->Montage_Play(FireAnimation, 1.f);
	    	}
	    }
        if (PlayerController != nullptr)
        {
            if (bIsAiming)
                PlayerController->ClientStartCameraShake(UMyLegacyCameraShake::StaticClass(), CurrentWeapon.Rebound * 0.5f);
            else
                PlayerController->ClientStartCameraShake(UMyLegacyCameraShake::StaticClass(), CurrentWeapon.Rebound);
        }
    }
}

void UWeaponComponent::FireLauncher()
{
    UE_LOG(LogTemp, Warning, TEXT("FireLauncher"));

    if(CurrentAmmo < 1)
    {
        StopFiring();
        return;
    }

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
        FVector MuzzleLocation;
        if(CurrentSkeletalMeshComponent)
        {
            MuzzleLocation = CurrentSkeletalMeshComponent->GetComponentLocation() + (CurrentSkeletalMeshComponent->GetRightVector() * 100.0f);
        }
        
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        ATitan_Skill_Grenade* Projectile = nullptr;
        if(!CurrentWeapon.Linetracing)
        {
		    Projectile = World->SpawnActor<ATitan_Skill_Grenade>(MuzzleLocation, SpawnRotation, ActorSpawnParams);
            Projectile->SetProjectile(true);
            Projectile->SetGrenadeDamage(100.0f);
        }
        
		const FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
		
		const FVector TraceStart = CameraLocation;
		const FVector TraceEnd = TraceStart + (SpawnRotation.Vector() * 1000000.0f);
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());
        if(!CurrentWeapon.Linetracing)
		    CollisionParams.AddIgnoredActor(Projectile);
		
        FCollisionObjectQueryParams ObjectQueryParams;
        ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		//bool bHit = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);
        bool bHit = World->LineTraceSingleByObjectType(HitResult, TraceStart, TraceEnd, ObjectQueryParams, CollisionParams);
		DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 1, 0, 1);
		FVector ProjectileDirection = SpawnRotation.Vector();
		
        if (FireAnimation != nullptr && !bIsAiming)
	    {
	    	ADestinyFPSBase* PlayerCharacter = Cast<ADestinyFPSBase>(GetOwner());
	    	UAnimInstance* AnimInstance = PlayerCharacter->GetFppMesh()->GetAnimInstance();
	    	if (AnimInstance != nullptr)
	    	{
	    		AnimInstance->Montage_Play(FireAnimation, 1.f);
	    	}
	    }

        if (PlayerController != nullptr)
    	{
            if(bIsAiming)
    		    PlayerController->ClientStartCameraShake(UMyLegacyCameraShake::StaticClass(), CurrentWeapon.Rebound * 0.5f);
            else
                PlayerController->ClientStartCameraShake(UMyLegacyCameraShake::StaticClass(), CurrentWeapon.Rebound);
   		}

		if (bHit)
		{
            if (HitResult.GetActor()->ActorHasTag("Enemy"))
            {
                DrawDebugSphere(World, HitResult.Location, 10.0f, 12, FColor::Green, false, 1.0f);
			    UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
			    UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *HitResult.Location.ToString());
                if(CurrentWeapon.Linetracing)
                {
                    UE_LOG(LogTemp, Warning, TEXT("linetracing"));
                    UE_LOG(LogTemp, Warning, TEXT("Damage : %f"),CurrentWeapon.GunDamage);
                    UGameplayStatics::ApplyDamage(HitResult.GetActor(), CurrentWeapon.GunDamage, PlayerController, GetOwner(), UDamageType::StaticClass());
                    return;
                }
                
            }
			ProjectileDirection = (HitResult.Location - MuzzleLocation).GetSafeNormal();			
		}
        if (Projectile)
        {
            Projectile->SetThrowDirection(ProjectileDirection);
        }
       
	}
}

void UWeaponComponent::StartFiring()
{
    UE_LOG(LogTemp, Warning, TEXT("StartFiring"));
    if(CurrentAmmo < 1)
    {
        StopFiring();
        StopAiming();
        Reload();
        return;
    }

    if(!IsReloading)
    {
        if (!bIsFiring)
        {
            bIsFiring = true;

            if (CurrentWeapon.AutoFire)
            {
                if(CurrentWeapon.GunType == GunTypeList::SHOTGUN)
                {
                    FireInRange();
                    GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UWeaponComponent::FireInRange, CurrentWeapon.FireRate, true);
                }
                else
                {
                    Fire(); // 첫 발사
                    GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UWeaponComponent::Fire, CurrentWeapon.FireRate, true);
                }
                UE_LOG(LogTemp, Warning, TEXT("autofire"));
            }
            else
            {
                if(CurrentWeapon.GunType == GunTypeList::SHOTGUN)
                {
                    FireInRange();
                }
                else if(CurrentWeapon.GunType == GunTypeList::LAUNCHER)
                {
                    FireLauncher();
                }
                else
                {
                    Fire(); // 단발 발사
                }
                UE_LOG(LogTemp, Warning, TEXT("onefire"));
            }
        }
    }
}

void UWeaponComponent::StopFiring()
{
    UE_LOG(LogTemp, Warning, TEXT("StopFiring"));
    if (bIsFiring)
    {
        bIsFiring = false;

        if (CurrentWeapon.AutoFire)
        {
            GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
        }
    }
}

void UWeaponComponent::StartAiming()
{
    UE_LOG(LogTemp, Warning, TEXT("StartAiming"));
    bIsAiming = true;
}

void UWeaponComponent::StopAiming()
{
    UE_LOG(LogTemp, Warning, TEXT("StopAiming"));
    bIsAiming = false;
}

void UWeaponComponent::EquipWeapon1()
{
	UE_LOG(LogTemp, Warning, TEXT("1"));
    LoadWeaponByName(FName("Pistol"));
    FString ModelPath = CurrentWeapon.GunModelPath;
    LoadAndAttachModelToCharacter(Cast<ADestinyFPSBase>(GetOwner()), ModelPath);
}

void UWeaponComponent::EquipWeapon2()
{
	UE_LOG(LogTemp, Warning, TEXT("2"));
    LoadWeaponByName(FName("Pistol2"));
    FString ModelPath = CurrentWeapon.GunModelPath;
    LoadAndAttachModelToCharacter(Cast<ADestinyFPSBase>(GetOwner()), ModelPath);   
}

void UWeaponComponent::EquipWeapon3()
{
	UE_LOG(LogTemp, Warning, TEXT("3"));
    LoadWeaponByName(FName("Pistol3"));
    FString ModelPath = CurrentWeapon.GunModelPath;
    LoadAndAttachModelToCharacter(Cast<ADestinyFPSBase>(GetOwner()), ModelPath);
}

void UWeaponComponent::Reload()
{
    if(!IsReloading)
    {
        IsReloading = true;
        if (ReloadAnimation != nullptr)
	    {
	    	ADestinyFPSBase* PlayerCharacter = Cast<ADestinyFPSBase>(GetOwner());
	    	UAnimInstance* AnimInstance = PlayerCharacter->GetFppMesh()->GetAnimInstance();
	    	if (AnimInstance != nullptr)
	    	{
	    		AnimInstance->Montage_Play(ReloadAnimation, 1.f);
	    	}
	    }
    }
}

void UWeaponComponent::FillAmmo()
{
    CurrentAmmo = MaxAmmo;
    AmmoWidget->UpdateAmmo(CurrentAmmo, MaxAmmo);
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
            //EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UWeaponComponent::Fire);
            EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UWeaponComponent::StartFiring);
            EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UWeaponComponent::StopFiring);
            EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &UWeaponComponent::StartAiming);
            EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &UWeaponComponent::StopAiming);
            EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UWeaponComponent::Reload);
            EnhancedInputComponent->BindAction(Equip1Action, ETriggerEvent::Started, this, &UWeaponComponent::EquipWeapon1);
            EnhancedInputComponent->BindAction(Equip2Action, ETriggerEvent::Started, this, &UWeaponComponent::EquipWeapon2);
            EnhancedInputComponent->BindAction(Equip3Action, ETriggerEvent::Started, this, &UWeaponComponent::EquipWeapon3);
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("addmapping"));
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


void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (CurrentSkeletalMeshComponent)
    {
        FVector TargetLocation;
        FRotator TargetRotation;

        if (bIsAiming)
        {
            CurrentScopeSize = FMath::FInterpTo(CurrentScopeSize, 1.0f, DeltaTime, ScopeZoomSpeed);
            CurrentScopeX = FMath::FInterpTo(CurrentScopeX, 0.0f, DeltaTime, ScopeZoomSpeed);
            if (AmmoWidget && CurrentWeapon.GunType == GunTypeList::RIFLE)
            {
                AmmoWidget->SetScopeSize(CurrentScopeSize);
                AmmoWidget->ImageMove(CurrentScopeX,CurrentSkeletalMeshComponent);
            }


            if (APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
            {
                FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
                FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();

                TargetLocation = CameraLocation + CameraRotation.Vector() * 50.0f;
                TargetLocation.Z -= 20.0f;
                TargetRotation = CameraRotation;
                TargetRotation.Yaw -= 90;


                float CurrentFOV = PlayerController->PlayerCameraManager->GetFOVAngle();
                float TargetFOV = 30.0f;
                float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, 5.0f);
                PlayerController->PlayerCameraManager->SetFOV(NewFOV);
            }
        }
        else
        {
            FVector HandLocation;
            FRotator HandRotation;
            if (Character->GetFppMesh())
            {
                HandLocation = Character->GetFppMesh()->GetSocketLocation(FName(TEXT("GripPoint")));
                HandRotation = Character->GetFppMesh()->GetSocketRotation(FName(TEXT("GripPoint")));
            }
            TargetLocation = HandLocation + DefaultOffset; 
            TargetRotation = HandRotation + DefaultRotation;

            if (APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
            {
                float CurrentFOV = PlayerController->PlayerCameraManager->GetFOVAngle();
                float TargetFOV = 90.0f;
                float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, 5.0f);
                PlayerController->PlayerCameraManager->SetFOV(NewFOV);
            }

            if (AmmoWidget && CurrentWeapon.GunType == GunTypeList::RIFLE)
            {
                CurrentScopeSize = 0.5f;
                CurrentScopeX = 600.0f;
                AmmoWidget->SetScopeSize(CurrentScopeSize);
                AmmoWidget->ImageMove(CurrentScopeX,CurrentSkeletalMeshComponent);
            }
        }

    FVector NewLocation = FMath::VInterpTo(CurrentSkeletalMeshComponent->GetComponentLocation(), TargetLocation, GetWorld()->GetDeltaSeconds(), AimingSpeed);
    FRotator NewRotation = FMath::RInterpTo(CurrentSkeletalMeshComponent->GetComponentRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), AimingSpeed);

    CurrentSkeletalMeshComponent->SetWorldLocation(NewLocation);
    CurrentSkeletalMeshComponent->SetWorldRotation(NewRotation);
    }
}