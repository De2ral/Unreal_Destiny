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

#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "Net/UnrealNetwork.h"



#include "Kismet/GameplayStatics.h"

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicated(true);
    Character = Cast<ADestinyFPSBase>(GetOwner());

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Sparks/P_Sparks_C.P_Sparks_C'"));
    if (ParticleAsset.Succeeded())
    {
        RifleFlash = ParticleAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset1(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_A.P_Explosion_Big_A'"));
    if (ParticleAsset1.Succeeded())
    {
        HitFlash = ParticleAsset1.Object;
    }

}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    ADestinyFPSBase* PlayerCharacter = Cast<ADestinyFPSBase>(GetOwner());
    
    AddMapping(PlayerCharacter);

    if(GetOwner()->HasAuthority())
    {
        if (AmmoWidgetClass)
	    {
            //CurrentAmmo = MaxAmmo;
	    	AmmoWidget = CreateWidget<UWeaponWidget>(GetWorld(), AmmoWidgetClass);
	    	if (AmmoWidget)
	    	{
	    		AmmoWidget->AddToViewport();
	    	}
	    }
    }
    else
    {
        if (AmmoWidgetClass2)
	    {
            //CurrentAmmo = MaxAmmo;
	    	AmmoWidget2 = CreateWidget<UWeaponWidget>(GetWorld(), AmmoWidgetClass2);
	    	if (AmmoWidget2)
	    	{
	    		AmmoWidget2->AddToViewport();
	    	}
	    }
    }
    
    //if (GetOwner()->HasAuthority())
    //{
    //    SetSlot1Weapon(TEXT("Rifle1"));
    //}

    if (PlayerCharacter != nullptr)
    {
        SetSlot1Weapon(TEXT("Rifle1"));
        SetSlot2Weapon(TEXT("Pistol1"));
        SetSlot3Weapon(TEXT("Launcher1"));

        EquipWeapon1();
    }
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter called failed."));
	}
    FillAmmo();

    ChangeCrosshair();
}



void UWeaponComponent::Fire()
{
    if (GetOwner()->HasAuthority())  // 서버에서 실행되는지 확인
    {
        ServerFire();
	}
    
    else
    {
        ServerFire();  // 클라이언트에서 호출 시 서버로 전달
    }
}
void UWeaponComponent::ServerFire_Implementation()
{
    MulticastTakeFire();
}
bool UWeaponComponent::ServerFire_Validate()
{
    return true;  // 보통 추가 검증이 필요할 수 있음
}
void UWeaponComponent::MulticastTakeFire_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Fire"));

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
        FRotator SpawnRotation = FRotator::ZeroRotator;
        if(PlayerController)
		    SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();

        FVector MuzzleLocation;

        if(CurrentStaticMeshComponent)
        {
            if(bIsAiming)
                MuzzleLocation = CurrentStaticMeshComponent->GetComponentLocation() + SpawnRotation.Vector() * 300.0f;
            else
                MuzzleLocation = CurrentStaticMeshComponent->GetComponentLocation() + (CurrentStaticMeshComponent->GetRightVector() * 100.0f);
        }
 
        

        if (RifleFlash && !bIsAiming)
        {
            FVector LeftOffset = -SpawnRotation.RotateVector(FVector::RightVector) * 10.0f;
            FVector UpOffset = SpawnRotation.RotateVector(FVector::UpVector) * 5.0f;
            FVector MuzzleFlashLocation = MuzzleLocation + LeftOffset + UpOffset;
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RifleFlash, MuzzleFlashLocation, FRotator(0.0f,0.0f, -45.0f), FVector(0.5f ,0.5f, 0.5f));
        }

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        AFpsCppProjectile* Projectile = nullptr;
        if(!CurrentWeapon.Linetracing)
        {
		    //Projectile = World->SpawnActor<AFpsCppProjectile>(MuzzleLocation, SpawnRotation, ActorSpawnParams);
            //if(Projectile)
            //{
            //    Projectile->SetProjectile(CurrentWeapon.ProjectileMesh, CurrentWeapon.ProjectileSpeed, CurrentWeapon.GunDamage);
            //    Projectile->AttachTrailEffect(true);
            //}

            Projectile = World->SpawnActorDeferred<AFpsCppProjectile>(AFpsCppProjectile::StaticClass(), FTransform(SpawnRotation, MuzzleLocation), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
            if (Projectile)
            {
                // 이 부분에서 SetProjectile을 생성 시 설정한 값을 전달합니다.
                Projectile->SetProjectile(CurrentWeapon.ProjectileMesh, CurrentWeapon.ProjectileSpeed, CurrentWeapon.GunDamage);

                // 트레일 이펙트나 추가 설정을 이 곳에서 할 수 있습니다.
                Projectile->AttachTrailEffect(true);

                // 최종적으로 스폰 완료를 알림
                UGameplayStatics::FinishSpawningActor(Projectile, FTransform(SpawnRotation, MuzzleLocation));
            }
        }
        
		FVector CameraLocation = FVector::ZeroVector;
		if(PlayerController)
            CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
		const FVector TraceStart = CameraLocation;
		const FVector TraceEnd = TraceStart + (SpawnRotation.Vector() * 1000000.0f);
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());
        if(!CurrentWeapon.Linetracing)
		    CollisionParams.AddIgnoredActor(Projectile);
		
        FCollisionObjectQueryParams ObjectQueryParams;
        ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
        ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
        ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		//bool bHit = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);
        bool bHit = World->LineTraceSingleByObjectType(HitResult, TraceStart, TraceEnd, ObjectQueryParams, CollisionParams);
		DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 1, 0, 1);
		FVector ProjectileDirection = SpawnRotation.Vector();
		
        if (FireAnimation != nullptr && !bIsAiming && (CurrentWeapon.GunType == GunTypeList::RIFLE))
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
            ProjectileDirection = (HitResult.Location - MuzzleLocation).GetSafeNormal();
            DrawDebugSphere(World, HitResult.Location, 10.0f, 12, FColor::Green, false, 1.0f);
           
            if(CurrentWeapon.Linetracing)
            {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFlash, HitResult.Location, FRotator(0.0f,0.0f, 0.0f), FVector(0.25f ,0.25f, 0.25f));
                if (HitResult.GetActor()->ActorHasTag("Enemy"))
                {
                    UGameplayStatics::ApplyDamage(HitResult.GetActor(), CurrentWeapon.GunDamage, PlayerController, GetOwner(), UDamageType::StaticClass());
                    return;
                }
            } 			
		}
        if (Projectile)
        {
            Projectile->GetProjectileMovement()->Velocity = ProjectileDirection * Projectile->GetProjectileMovement()->InitialSpeed;
        }
    }
}

void UWeaponComponent::FireInRange()
{
    if (GetOwner()->HasAuthority())
    {
        ServerFireInRange();
    }
    
    else
    {
        ServerFireInRange();
    }
    
}
void UWeaponComponent::ServerFireInRange_Implementation()
{
    MulticastTakeFireInRange();
}
bool UWeaponComponent::ServerFireInRange_Validate()
{
    return true;
}
void UWeaponComponent::MulticastTakeFireInRange_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("FireInRange"));

    UWorld* const World = GetWorld();
    if (World != nullptr)
    {
        APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
        
        const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
        FVector MuzzleLocation;
        if (CurrentStaticMeshComponent)
        {
            MuzzleLocation = CurrentStaticMeshComponent->GetComponentLocation() + (CurrentStaticMeshComponent->GetRightVector() * 100.0f);
        }

        const FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
        const FVector TraceStart = CameraLocation;
        
        const int32 NumPellets = 10; // 총알의 개수
        const float SpreadAngle = 5.0f; // 발사 범위 각도

        for (int32 i = 0; i < NumPellets; ++i)
        {
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, i, SpawnRotation, TraceStart, SpreadAngle, World, PlayerController]()
            {
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
                CollisionParams.bTraceComplex = true;

                FCollisionObjectQueryParams ObjectQueryParams;
                ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
                bool bHit = World->LineTraceSingleByObjectType(HitResult, TraceStart, PelletEnd, ObjectQueryParams, CollisionParams);
                DrawDebugLine(World, TraceStart, PelletEnd, FColor::Red, false, 1, 0, 1);

                if (bHit)
                {

                    if(CurrentWeapon.Linetracing)
                    {
                        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFlash, HitResult.Location, FRotator(0.0f,0.0f, 0.0f), FVector(0.25f ,0.25f, 0.25f));
                        if (HitResult.GetActor()->ActorHasTag("Enemy"))
                        {
                            DrawDebugSphere(World, HitResult.Location, 10.0f, 12, FColor::Green, false, 1.0f);
                            UGameplayStatics::ApplyDamage(HitResult.GetActor(), CurrentWeapon.GunDamage * 0.1f, PlayerController, GetOwner(), UDamageType::StaticClass());
                            return;
                        }
                    } 	
                }
            }, i * 0.001f, false);

            
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
    if (GetOwner()->HasAuthority())
    {
        ServerFireLauncher();
    }
    else
    {
        ServerFireLauncher();
    }
       
	
}
void UWeaponComponent::ServerFireLauncher_Implementation()
{
    MulticastTakeFireLauncher();
}
bool UWeaponComponent::ServerFireLauncher_Validate()
{
    return true;  // 보통 추가 검증이 필요할 수 있음
}
void UWeaponComponent::MulticastTakeFireLauncher_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("FireLauncher"));
 
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
        if (PlayerController == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController is null."));
        return;
    }
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
        FVector MuzzleLocation;
        if(CurrentStaticMeshComponent)
        {
            MuzzleLocation = CurrentStaticMeshComponent->GetComponentLocation() + (CurrentStaticMeshComponent->GetRightVector() * 300.0f);
        }
        
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        AFpsCppProjectile* Projectile = nullptr;
        if(!CurrentWeapon.Linetracing)
        {
		    Projectile = World->SpawnActor<AFpsCppProjectile>(MuzzleLocation, SpawnRotation, ActorSpawnParams);
            if(Projectile)
            {
                Projectile->SetProjectile(CurrentWeapon.ProjectileMesh, CurrentWeapon.ProjectileSpeed, CurrentWeapon.GunDamage);
                Projectile->SetbExplodeOnImpact(true);
                Projectile->AttachTrailEffect(false);
            }
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
        ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
        ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		//bool bHit = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);
        bool bHit = World->LineTraceSingleByObjectType(HitResult, TraceStart, TraceEnd, ObjectQueryParams, CollisionParams);
		DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 1, 0, 1);
        DrawDebugSphere(World, HitResult.Location, 10.0f, 12, FColor::Green, false, 1.0f);
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

void UWeaponComponent::StartFiring()
{
    UseAmmo();
    if(!Character->isUltimate && !Character->isMeleeAttack && !Character->isGrenade)
    {
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
                }
            }
        }
    }
}

void UWeaponComponent::StopFiring()
{
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
    if(GetOwner()->HasAuthority())
        AmmoWidget->SetTextureBasedOnGunType(int(CurrentWeapon.GunType),true);
    else
        AmmoWidget2->SetTextureBasedOnGunType(int(CurrentWeapon.GunType),true);

    if(CurrentScopeX <= 0.1f)
    {
        FVector MeshTargetLocation = Character->GetFppMesh()->GetRelativeLocation();
        FRotator MeshTargetRotation;

        MeshTargetLocation.Y = CurrentWeapon.AimLocation;
        MeshTargetLocation.Z = -160.0f - 1.0f;
        MeshTargetRotation.Yaw = CurrentWeapon.AimRotation;

        APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
        FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
        FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
        FVector ForwardVector = CameraRotation.Vector();
        FVector RightVector = FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::Y);
        FVector UpVector = FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::Z);
        
        FVector TargetLocation = CameraLocation + (ForwardVector * 50.0f); 
        TargetLocation -= UpVector * CurrentWeapon.AimLocation;

        FQuat QuatRotation = FQuat(CameraRotation);
        FRotator TargetRotation = QuatRotation.Rotator();
        QuatRotation = QuatRotation * FQuat(FRotator(0.0f, CurrentWeapon.AimRotation, 0.0f)); // 기존 Yaw 조정 부분을 쿼터니언으로 처리
        TargetRotation = QuatRotation.Rotator();

        if(CurrentWeapon.GunType == GunTypeList::PISTOL)
        {
            Character->GetFppMesh()->SetRelativeLocation(MeshTargetLocation);
            Character->GetFppMesh()->SetRelativeRotation(MeshTargetRotation);
        }
        else
        {
            CurrentStaticMeshComponent->SetWorldLocation(TargetLocation);
            CurrentStaticMeshComponent->SetWorldRotation(TargetRotation);
        }
    }
}

void UWeaponComponent::StopAiming()
{
    UE_LOG(LogTemp, Warning, TEXT("StopAiming"));
    bIsAiming = false;
    if(GetOwner()->HasAuthority())
        AmmoWidget->SetTextureBasedOnGunType(int(CurrentWeapon.GunType),false);
    else
        AmmoWidget2->SetTextureBasedOnGunType(int(CurrentWeapon.GunType),false);
    //APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
    //PlayerController->InputYawScale_DEPRECATED = OriginalMouseSensitivity;  // 감도 감소 (0.5배로)
    return;
}

void UWeaponComponent::EquipWeapon1()
{
    if(GetOwner()->HasAuthority())
    {
        
        ServerEquipWeapon1();
        //ChangeCrosshair();
        
    }
    else
    {
        ServerEquipWeapon1();
        //ChangeCrosshair();
    }
}
bool UWeaponComponent::ServerEquipWeapon1_Validate()
{
    // 필요 시 유효성 검사 추가
    return true;
}
void UWeaponComponent::ServerEquipWeapon1_Implementation()
{
    MulticastEquipWeapon1();
}
void UWeaponComponent::MulticastEquipWeapon1_Implementation()
{
    if(!bIsAiming)
    {
    LoadWeaponByName(Slot1Weapon);
    FString ModelPath = CurrentWeapon.GunModelPath;
    LoadAndAttachModelToCharacter(Cast<ADestinyFPSBase>(GetOwner()), ModelPath); 
    }
}

void UWeaponComponent::EquipWeapon2()
{
    if(GetOwner()->HasAuthority())
    {
        
        
        ServerEquipWeapon2();
        //ChangeCrosshair();
        
    }
    else
    {
        ServerEquipWeapon2();
        //ChangeCrosshair();
    }
}
bool UWeaponComponent::ServerEquipWeapon2_Validate()
{
    // 필요 시 유효성 검사 추가
    return true;
}
void UWeaponComponent::ServerEquipWeapon2_Implementation()
{
    MulticastEquipWeapon2();
}
void UWeaponComponent::MulticastEquipWeapon2_Implementation()
{
    if(!bIsAiming)
    {
    LoadWeaponByName(Slot2Weapon);
    FString ModelPath = CurrentWeapon.GunModelPath;
    LoadAndAttachModelToCharacter(Cast<ADestinyFPSBase>(GetOwner()), ModelPath); 
    }
}

void UWeaponComponent::EquipWeapon3()
{
    if(GetOwner()->HasAuthority())
    {
        ServerEquipWeapon3();
    }
    else
    {
        ServerEquipWeapon3();
        //ChangeCrosshair();
    }
}
bool UWeaponComponent::ServerEquipWeapon3_Validate()
{
    // 필요 시 유효성 검사 추가
    return true;
}
void UWeaponComponent::ServerEquipWeapon3_Implementation()
{
    MulticastEquipWeapon3();
}
void UWeaponComponent::MulticastEquipWeapon3_Implementation()
{
    if(!bIsAiming)
    {
    LoadWeaponByName(Slot3Weapon);
    FString ModelPath = CurrentWeapon.GunModelPath;
    LoadAndAttachModelToCharacter(Cast<ADestinyFPSBase>(GetOwner()), ModelPath); 
    }
}
void UWeaponComponent::ChangeCrosshair()
{
    UE_LOG(LogTemp, Warning, TEXT("ChangeCrosshair"));
    if(GetOwner()->HasAuthority())
    {
        AmmoWidget->SetTextureBasedOnGunType(int(CurrentWeapon.GunType), false);
        AmmoWidget->UpdateAmmo(CurrentAmmo(), StoredAmmo());
    }
    else
    {
        AmmoWidget2->SetTextureBasedOnGunType(int(CurrentWeapon.GunType), false);
        AmmoWidget2->UpdateAmmo(CurrentAmmo(), StoredAmmo());
    }
    
}

void UWeaponComponent::SetSlot1Weapon(FName inweapon)
{
    Slot1Weapon = inweapon;
    EquipWeapon1();
    AllFillAmmo();
}

void UWeaponComponent::SetSlot2Weapon(FName inweapon)
{
    Slot2Weapon = inweapon;
    EquipWeapon2();
    AllFillAmmo();
}

void UWeaponComponent::SetSlot3Weapon(FName inweapon)
{
    Slot3Weapon = inweapon;
    EquipWeapon3();
    AllFillAmmo();
}

int UWeaponComponent::CurrentAmmo()
{
    int CurrentTempAmmo = 0;
    
    if(CurrentWeapon.GunName == Slot1Weapon)
        CurrentTempAmmo = Ammo1;
    else if(CurrentWeapon.GunName == Slot2Weapon)
        CurrentTempAmmo = Ammo2;
    else if(CurrentWeapon.GunName == Slot3Weapon)
        CurrentTempAmmo = Ammo3;      

    return CurrentTempAmmo;
}

int UWeaponComponent::StoredAmmo()
{
    int StoredTempAmmo = 0;
    switch (CurrentWeapon.BulletType)
    {
        case BulletTypeList::REGULAR:
            StoredTempAmmo = StoredAmmo_Regular;
            break;
        case BulletTypeList::SPECIAL:
            StoredTempAmmo = StoredAmmo_Special;
            break;
        case BulletTypeList::REINFORCE:
            StoredTempAmmo = StoredAmmo_Reinforce;
            break;
    }
    return StoredTempAmmo;
}

void UWeaponComponent::UseAmmo()
{
    if(CurrentWeapon.GunName == Slot1Weapon)
    {
        if(Ammo1 < 1)
        {
            StopFiring();
            StopAiming();
            Reload();
            return;
        }
        Ammo1--;
    }
    else if(CurrentWeapon.GunName == Slot2Weapon)
    {
        if(Ammo2 < 1)
        {
            StopFiring();
            StopAiming();
            Reload();
            return;
        }
        Ammo2--;
    }
    else if(CurrentWeapon.GunName == Slot3Weapon)
    {
        if(Ammo3 < 1)
        {
            StopFiring();
            StopAiming();
            Reload();
            return;
        }
        Ammo3--;
    }
    if(GetOwner()->HasAuthority())
        AmmoWidget->UpdateAmmo(CurrentAmmo(), StoredAmmo());
    else
        AmmoWidget2->UpdateAmmo(CurrentAmmo(), StoredAmmo());
}

void UWeaponComponent::Reload()
{
    if(!Character->isUltimate && !Character->isMeleeAttack && !Character->isGrenade)
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
}

void UWeaponComponent::FillAmmo()
{
    if(CurrentWeapon.GunName == Slot1Weapon)
    {
        UE_LOG(LogTemp, Warning, TEXT("Slot1Weapon"));
        Ammo1 = CurrentWeapon.Max_capacity;
    }
        
    else if(CurrentWeapon.GunName == Slot2Weapon)
    {
        Ammo2 = CurrentWeapon.Max_capacity;
    }
       
    else if(CurrentWeapon.GunName == Slot3Weapon)
    {
        Ammo3 = CurrentWeapon.Max_capacity;
    }

    switch (CurrentWeapon.BulletType)
    {
        case BulletTypeList::REGULAR:
        {
            StoredAmmo_Regular -= CurrentWeapon.Max_capacity;
            break;
        }
        case BulletTypeList::SPECIAL:
        {
            StoredAmmo_Special -= CurrentWeapon.Max_capacity;
            break;
        }
            
        case BulletTypeList::REINFORCE:
        {
            StoredAmmo_Reinforce -= CurrentWeapon.Max_capacity;
            break;
        } 
    }

    if(GetOwner()->HasAuthority())
        AmmoWidget->UpdateAmmo(CurrentAmmo(), StoredAmmo());
    else
        AmmoWidget2->UpdateAmmo(CurrentAmmo(), StoredAmmo());
}

void UWeaponComponent::AllFillAmmo()
{
    FGunInfo* WeaponData = WeaponDataTable->FindRow<FGunInfo>(Slot1Weapon, TEXT(""));
    
    if(WeaponData)
        Ammo1 = WeaponData->Max_capacity;
    
    WeaponData = WeaponDataTable->FindRow<FGunInfo>(Slot2Weapon, TEXT(""));
    if(WeaponData)
        Ammo2 = WeaponData->Max_capacity;
        
    WeaponData = WeaponDataTable->FindRow<FGunInfo>(Slot3Weapon, TEXT(""));
    if(WeaponData)
        Ammo3 = WeaponData->Max_capacity;
       
    if(GetOwner()->HasAuthority())
        AmmoWidget->UpdateAmmo(CurrentAmmo(), StoredAmmo());
    else
        AmmoWidget2->UpdateAmmo(CurrentAmmo(), StoredAmmo());
}

void UWeaponComponent::ChangeGunPose(int inbool)
{
    FVector MeshLocation;
    FRotator MeshRotation;
    if(inbool == int(GunTypeList::PISTOL))
    {
        MeshLocation = FVector(-35, 15, -160);
        MeshRotation = FRotator(0, 0, 0);
        Character->GetFppMesh()->SetVisibility(true);
    }

    else if(inbool == int(GunTypeList::LAUNCHER))
    {
        MeshLocation = FVector(-20, 10, -130);
        MeshRotation = FRotator(0, 0, 0);
        Character->GetFppMesh()->SetVisibility(false);
    }

    else
    {
        MeshLocation = FVector(-20, 0, -150);
        MeshRotation = FRotator(0, 0, 0);
        Character->GetFppMesh()->SetVisibility(true);
    }
    Character->GetFppMesh()->SetRelativeLocation(MeshLocation);
    Character->GetFppMesh()->SetRelativeRotation(MeshRotation);
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
            EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &UWeaponComponent::StartAiming);
            EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &UWeaponComponent::StopAiming);
            EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UWeaponComponent::Reload);
            EnhancedInputComponent->BindAction(Equip1Action, ETriggerEvent::Started, this, &UWeaponComponent::EquipWeapon1);
            //EnhancedInputComponent->BindAction(Equip1Action, ETriggerEvent::Started, this, &UWeaponComponent::ChangeCrosshair);
            EnhancedInputComponent->BindAction(Equip2Action, ETriggerEvent::Started, this, &UWeaponComponent::EquipWeapon2);
            //EnhancedInputComponent->BindAction(Equip2Action, ETriggerEvent::Started, this, &UWeaponComponent::ChangeCrosshair);
            EnhancedInputComponent->BindAction(Equip3Action, ETriggerEvent::Started, this, &UWeaponComponent::EquipWeapon3);
            //EnhancedInputComponent->BindAction(Equip3Action, ETriggerEvent::Started, this, &UWeaponComponent::ChangeCrosshair);
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
        CurrentStaticMeshComponent_TPP = CurrentStaticMeshComponent;
        CurrentStaticMeshComponent->SetStaticMesh(StaticMesh);
        FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
        if(CurrentWeapon.GunType == GunTypeList::PISTOL)
        {
            ChangeGunPose(int(GunTypeList::PISTOL));
            CurrentStaticMeshComponent->AttachToComponent(Character->GetFppMesh(), AttachmentRules, FName(TEXT("GripPointPistol")));
        }

        else if(CurrentWeapon.GunType == GunTypeList::LAUNCHER)
        {
            ChangeGunPose(int(GunTypeList::LAUNCHER));
            CurrentStaticMeshComponent->AttachToComponent(Character->GetFppMesh(), AttachmentRules, FName(TEXT("GripPoint")));
        }
        else
        {
            ChangeGunPose(int(GunTypeList::RIFLE));
            CurrentStaticMeshComponent->AttachToComponent(Character->GetFppMesh(), AttachmentRules, FName(TEXT("GripPoint")));
        }
        CurrentStaticMeshComponent->RegisterComponent();

        UE_LOG(LogTemp, Warning, TEXT("StaticMesh"));

        DefaultOffset =  CurrentStaticMeshComponent->GetRelativeLocation();
        DefaultRotation = CurrentStaticMeshComponent->GetRelativeRotation();

        //ChangeCrosshair();
    }
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
    if(GetOwner()->HasAuthority())
    {
        CurrentWeapon = NewWeapon;
        if(CurrentWeapon.GunType == GunTypeList::PISTOL)
            Character->SetHasRifle(false);
        else
            Character->SetHasRifle(true);
    }
}

void UWeaponComponent::LoadWeaponByName(FName WeaponName)
{
    if (!GetOwner()->HasAuthority())
    {
        // 서버 권한이 없으면 함수 종료
        return;
    }

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

void UWeaponComponent::SetCurrentWeaponMeshVisibility(bool isVisible)
{
    if(CurrentSkeletalMeshComponent)
        CurrentSkeletalMeshComponent->SetVisibility(isVisible);
    if(CurrentStaticMeshComponent)
        CurrentStaticMeshComponent->SetVisibility(isVisible);
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

    if (CurrentStaticMeshComponent)
    {
        FVector TargetLocation;
        FRotator TargetRotation;

        FVector MeshTargetLocation = Character->GetFppMesh()->GetRelativeLocation();
        FRotator MeshTargetRotation;

        if (bIsAiming)
        {
            MeshTargetLocation.Y = CurrentWeapon.AimLocation;
            MeshTargetLocation.Z = -160.0f - 1.0f;
            MeshTargetRotation.Yaw = CurrentWeapon.AimRotation;

            CurrentScopeSize = FMath::FInterpTo(CurrentScopeSize, 1.0f, DeltaTime, ScopeZoomSpeed);
            CurrentScopeX = FMath::FInterpTo(CurrentScopeX, 0.0f, DeltaTime, ScopeZoomSpeed);

            if (AmmoWidget)
            {
                if(CurrentWeapon.GunType == GunTypeList::SNIPER)
                {
                    if(GetOwner()->HasAuthority())
                    {
                        AmmoWidget->SetScopeSize(CurrentScopeSize);
                        AmmoWidget->ImageMove(CurrentScopeX,CurrentStaticMeshComponent,true);
                    }
                    else
                    {
                        AmmoWidget2->SetScopeSize(CurrentScopeSize);
                        AmmoWidget2->ImageMove(CurrentScopeX,CurrentStaticMeshComponent,true);
                    }
                }
                else if(CurrentWeapon.GunType == GunTypeList::LAUNCHER)
                {
                    if(GetOwner()->HasAuthority())
                    {
                        AmmoWidget->SetScopeSize(CurrentScopeSize);
                        AmmoWidget->ImageMove(CurrentScopeX,CurrentStaticMeshComponent,false);
                    }
                    else
                    {
                        AmmoWidget2->SetScopeSize(CurrentScopeSize);
                        AmmoWidget2->ImageMove(CurrentScopeX,CurrentStaticMeshComponent,false);
                    }
                    
                } 
            }


            if (APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
            {
                FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
                FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();

                FVector ForwardVector = CameraRotation.Vector();
                FVector RightVector = FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::Y);
                FVector UpVector = FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::Z);

                TargetLocation = CameraLocation + (ForwardVector * 50.0f); 
        
                TargetLocation -= UpVector * CurrentWeapon.AimLocation; 

                FQuat QuatRotation = FQuat(CameraRotation);

                TargetRotation = QuatRotation.Rotator();
                QuatRotation = QuatRotation * FQuat(FRotator(0.0f, CurrentWeapon.AimRotation, 0.0f)); // 기존 Yaw 조정 부분을 쿼터니언으로 처리
                TargetRotation = QuatRotation.Rotator();
                
                
                float CurrentFOV = PlayerController->PlayerCameraManager->GetFOVAngle();
                float TargetFOV = CurrentWeapon.CameraZoom;
                float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, 5.0f);
                PlayerController->PlayerCameraManager->SetFOV(NewFOV);
            }
        }
        else
        {
            MeshTargetLocation.Y = 15.0f;
            MeshTargetLocation.Z = -160.0f;
            MeshTargetRotation.Yaw = 0.0f;
            CurrentScopeX = 1.0f;
            FVector HandLocation;
            FRotator HandRotation;
            if (Character->GetFppMesh())
            {
                if(CurrentWeapon.GunType != GunTypeList::PISTOL)
                {
                    HandLocation = Character->GetFppMesh()->GetSocketLocation(FName(TEXT("GripPoint")));
                    HandRotation = Character->GetFppMesh()->GetSocketRotation(FName(TEXT("GripPoint")));
                }
                else
                {
                    HandLocation = Character->GetFppMesh()->GetSocketLocation(FName(TEXT("GripPointPistol")));
                    HandRotation = Character->GetFppMesh()->GetSocketRotation(FName(TEXT("GripPointPistol")));
                }
                
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

            if (AmmoWidget)
            {
                if(CurrentWeapon.GunType == GunTypeList::SNIPER)
                {
                    CurrentScopeSize = 0.5f;
                    CurrentScopeX = 600.0f;
                    if(GetOwner()->HasAuthority())
                    {
                        AmmoWidget->SetScopeSize(CurrentScopeSize);
                        AmmoWidget->ImageMove(CurrentScopeX,CurrentStaticMeshComponent,true);
                    }
                    else
                    {
                        AmmoWidget2->SetScopeSize(CurrentScopeSize);
                        AmmoWidget2->ImageMove(CurrentScopeX,CurrentStaticMeshComponent,true);
                    }                    
                }
                else if(CurrentWeapon.GunType == GunTypeList::LAUNCHER)
                {
                    CurrentScopeSize = 0.5f;
                    CurrentScopeX = 600.0f;
                    if(GetOwner()->HasAuthority())
                    {
                        AmmoWidget->SetScopeSize(CurrentScopeSize);
                        AmmoWidget->ImageMove(CurrentScopeX,CurrentStaticMeshComponent,false);
                    }
                    else
                    {
                        AmmoWidget2->SetScopeSize(CurrentScopeSize);
                        AmmoWidget2->ImageMove(CurrentScopeX,CurrentStaticMeshComponent,false);
                    }
                    
                } 
            }
        }


    APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
    FVector NewLocation = FVector::ZeroVector;
    FRotator NewRotation = FRotator::ZeroRotator;

    FVector MeshNewLocation = FVector::ZeroVector; 
    FRotator MeshNewRotation = FRotator::ZeroRotator;

    

    if(CurrentScopeX > 0.1f)
    {
        NewLocation = FMath::VInterpTo(CurrentStaticMeshComponent->GetComponentLocation(), TargetLocation, GetWorld()->GetDeltaSeconds(), AimingSpeed);
        NewRotation = FMath::RInterpTo(CurrentStaticMeshComponent->GetComponentRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), AimingSpeed);

        MeshNewLocation = FMath::VInterpTo(Character->GetFppMesh()->GetRelativeLocation(), MeshTargetLocation, GetWorld()->GetDeltaSeconds(), AimingSpeed);
        MeshNewRotation = FMath::RInterpTo(Character->GetFppMesh()->GetRelativeRotation(), MeshTargetRotation, GetWorld()->GetDeltaSeconds(), AimingSpeed);

        if(CurrentWeapon.GunType == GunTypeList::PISTOL)
        {
            Character->GetFppMesh()->SetRelativeLocation(MeshNewLocation);
            Character->GetFppMesh()->SetRelativeRotation(MeshNewRotation);
        }
        else
        {
            CurrentStaticMeshComponent->SetWorldLocation(NewLocation);
            CurrentStaticMeshComponent->SetWorldRotation(NewRotation);
        }
    }

    else
    {
        //NewLocation = FMath::VInterpTo(CurrentStaticMeshComponent->GetComponentLocation(), TargetLocation, GetWorld()->GetDeltaSeconds(), 50.0f);
        //NewRotation = FMath::RInterpTo(CurrentStaticMeshComponent->GetComponentRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 50.0f);
//
        //MeshNewLocation = FMath::VInterpTo(Character->GetFppMesh()->GetRelativeLocation(), MeshTargetLocation, GetWorld()->GetDeltaSeconds(), 50.0f);
        //MeshNewRotation = FMath::RInterpTo(Character->GetFppMesh()->GetRelativeRotation(), MeshTargetRotation, GetWorld()->GetDeltaSeconds(), 50.0f);
        //NewLocation = TargetLocation;
        //NewRotation = TargetRotation;
        //MeshNewLocation = MeshTargetLocation;
        //MeshNewRotation = MeshTargetRotation;
    }
    }
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //if(CurrentWeapon.ProjectileMesh != nullptr)
    DOREPLIFETIME(UWeaponComponent, CurrentWeapon);    
}

void UWeaponComponent::OnRep_CurrentWeapon()
{
    UE_LOG(LogTemp, Log, TEXT("OnRep_CurrentWeapon called"));
    ADestinyFPSBase* PlayerCharacter = Cast<ADestinyFPSBase>(GetOwner());
    if (PlayerCharacter)
    {
        UE_LOG(LogTemp, Log, TEXT("PlayerCharacter called"));
        LoadWeaponByName(CurrentWeapon.GunName);
        LoadAndAttachModelToCharacter(PlayerCharacter, CurrentWeapon.GunModelPath);
    }
}