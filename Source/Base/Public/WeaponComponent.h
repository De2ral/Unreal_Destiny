#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Engine/DataTable.h"
#include "WeaponDataManage.h"


#include "WeaponWidget.h"
#include "WeaponComponent.generated.h"

class ADestinyFPSBase;
class UStaticMesh;
class USkeletalMesh;
class AFpsCppProjectile;
class ATitan_Skill_Grenade;


//UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASE_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeaponComponent();

    // 투사체
	//UPROPERTY(EditAnywhere, Category=Projectile, BlueprintReadWrite)
	//TSubclassOf<class AFpsCppProjectile> ProjectileClass;

    // 투사체 발사 위치
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	//FVector MuzzleOffset;

    // 모델 장착
    void AttachModelToCharacter(ADestinyFPSBase* TargetCharacter, UObject* Model);

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void AttachSelectedModelToCharacter(class ADestinyFPSBase* InCharacter, UObject* SelectedModel);

	void LoadAndAttachModelToCharacter(ADestinyFPSBase* InCharacter, const FString& ModelPath);

    // 무기 장착


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    UDataTable* WeaponDataTable;

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetCurrentWeapon(const FGunInfo& NewWeapon);

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void LoadWeaponByName(FName WeaponName);

    void RemoveCurrentWeaponModel();

    // 키 입력 추가
    void AddMapping(ADestinyFPSBase* TargetCharacter);

    // 위젯
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UWeaponWidget> AmmoWidgetClass;

    UPROPERTY()
    UWeaponWidget* AmmoWidget;

    //UPROPERTY(EditAnywhere, Category = "UI")
    //TSubclassOf<UWeaponWidget> AmmoWidgetClass2;

    //UPROPERTY()
    //UWeaponWidget* AmmoWidget2;



    // 애니메이션
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* ReloadAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;


    
    //UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite, Category = "Weapon")
	//float DamageAmount;

  

protected:
    virtual void BeginPlay() override;

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    
    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputMappingContext* FireMappingContext;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* FireAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* AimAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* ReloadAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* Equip1Action;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* Equip2Action;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* Equip3Action;

    void Fire();
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerFire();
    UFUNCTION(NetMulticast, Reliable)
	void MulticastTakeFire();

    void FireInRange();
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerFireInRange();
    UFUNCTION(NetMulticast, Reliable)
	void MulticastTakeFireInRange();

    void FireLauncher();
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerFireLauncher();
    UFUNCTION(NetMulticast, Reliable)
	void MulticastTakeFireLauncher();

    void StartFiring();
    void StopFiring();

    void StartAiming();
    void StopAiming();

    void UseAmmo();

    
    void Reload();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    bool GetIsAiming() {return bIsAiming;}

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void FillAmmo();
    
    void AllFillAmmo();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void EndReloading();

    
    void EquipWeapon1();
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerEquipWeapon1();
    UFUNCTION(NetMulticast, Reliable)
	void MulticastEquipWeapon1();

    void EquipWeapon2();
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerEquipWeapon2();
    UFUNCTION(NetMulticast, Reliable)
	void MulticastEquipWeapon2();

    void EquipWeapon3();
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerEquipWeapon3();
    UFUNCTION(NetMulticast, Reliable)
	void MulticastEquipWeapon3();

    void ChangeCrosshair();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetSlot1Weapon(FName inweapon);
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetSlot2Weapon(FName inweapon);
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetSlot3Weapon(FName inweapon);

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    FName GetSlot1Weapon() {return Slot1Weapon;}
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    FName GetSlot2Weapon() {return Slot2Weapon;}
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    FName GetSlot3Weapon() {return Slot3Weapon;}

    int CurrentAmmo();
    int StoredAmmo(FName SlotWeapon);

    void ChangeGunPose(int inbool);

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
    bool IsReloading = false;
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerIsReloading(bool value);

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
    bool bIsFiring = false;
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerIsFiring(bool value);
    //int32 CurrentAmmo;
    //int32 MaxAmmo = 30;

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetCurrentWeaponMeshVisibility(bool isVisible);

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    UStaticMeshComponent* GetCurrentStaticMeshComponent_TPP() {return CurrentStaticMeshComponent_TPP;}
public:
    ADestinyFPSBase* Character;
    
    UPROPERTY(ReplicatedUsing=OnRep_CurrentWeapon, BlueprintReadOnly)
    FGunInfo CurrentWeapon;

    UFUNCTION()
    void OnRep_CurrentWeapon();

    bool bIsAiming;

    FName Slot1Weapon;
    FName Slot2Weapon;
    FName Slot3Weapon;

    UPROPERTY(Replicated, BlueprintReadOnly)
    FName SubWeaponName1;

    UPROPERTY(Replicated, BlueprintReadOnly)
    FName SubWeaponName2;

    int Ammo1 = 0;
    int Ammo2 = 0;
    int Ammo3 = 0;

    int StoredAmmo_Regular = 100;
    int StoredAmmo_Special = 150;
    int StoredAmmo_Reinforce = 300;

    UStaticMeshComponent* CurrentStaticMeshComponent;
    UStaticMeshComponent* CurrentStaticMeshComponent_TPP;

    USkeletalMeshComponent* CurrentSkeletalMeshComponent;
    UStaticMeshComponent* CurrentMultiStaticMeshComponent;
    USkeletalMeshComponent* CurrentMultiSkeletalMeshComponent;

    FVector DefaultOffset;
    FRotator DefaultRotation;

    float AimingSpeed = 5.0f;

    FTimerHandle FireTimerHandle;  

     // 스코프 확대 속도
    float ScopeZoomSpeed = 2.0f;

    // 현재 스코프 크기
    float CurrentScopeSize = 0.0f;

    float CurrentScopeX = 1.0f;

    UParticleSystem* PistolFlash;
    UParticleSystem* RifleFlash;
    UParticleSystem* SniperFlash;
    UParticleSystem* LauncherFlash;
    
    UParticleSystem* HitFlash;
};