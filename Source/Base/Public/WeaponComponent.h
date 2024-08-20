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

//UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASE_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeaponComponent();

    // 투사체
	UPROPERTY(EditAnywhere, Category=Projectile, BlueprintReadWrite)
	TSubclassOf<class AFpsCppProjectile> ProjectileClass;



    // 투사체 발사 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

   
    // 모델 장착
    void AttachModelToCharacter(ADestinyFPSBase* TargetCharacter, UObject* Model);

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void AttachSelectedModelToCharacter(class ADestinyFPSBase* InCharacter, UObject* SelectedModel);

	void LoadAndAttachModelToCharacter(ADestinyFPSBase* InCharacter, const FString& ModelPath);

    // 무기 장착
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FGunInfo CurrentWeapon;

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

    // 애니메이션
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* ReloadAnimation;
    
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
    void FireInRange();

    void StartFiring();
    void StopFiring();

    void StartAiming();
    void StopAiming();

    void Reload();

    void EquipWeapon1();
    void EquipWeapon2();
    void EquipWeapon3();

    int32 CurrentAmmo;
    int32 MaxAmmo = 30;
    
private:
    ADestinyFPSBase* Character;
    
    UStaticMeshComponent* CurrentStaticMeshComponent;
    USkeletalMeshComponent* CurrentSkeletalMeshComponent;

    bool bIsAiming = false;
    bool bIsFiring = false;                

    FVector DefaultOffset;
    FRotator DefaultRotation;

    float AimingSpeed = 5.0f;

    FTimerHandle FireTimerHandle;  

     // 스코프 확대 속도
    float ScopeZoomSpeed = 2.0f;

    // 현재 스코프 크기
    float CurrentScopeSize = 0.0f;

    float CurrentScopeX = 1.0f;
};