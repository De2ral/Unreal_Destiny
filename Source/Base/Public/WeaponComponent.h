#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Engine/DataTable.h"
#include "WeaponDataManage.h"


#include "TestWidget.h"
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

	UPROPERTY(EditAnywhere, Category=Projectile, BlueprintReadWrite)
	TSubclassOf<class AFpsCppProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

   

    void AttachModelToCharacter(ADestinyFPSBase* TargetCharacter, UObject* Model);

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void AttachSelectedModelToCharacter(class ADestinyFPSBase* InCharacter, UObject* SelectedModel);

	void LoadAndAttachModelToCharacter(ADestinyFPSBase* InCharacter, const FString& ModelPath);


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FGunInfo CurrentWeapon;

    // 데이터 테이블 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    UDataTable* WeaponDataTable;

    // 무기 데이터를 설정하는 함수
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetCurrentWeapon(const FGunInfo& NewWeapon);

    // 무기 데이터를 이름으로 가져오는 함수
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void LoadWeaponByName(FName WeaponName);

    void AddMapping(ADestinyFPSBase* TargetCharacter);

    void RemoveCurrentWeaponModel();

protected:
    virtual void BeginPlay() override;

     virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    ADestinyFPSBase* Character;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputMappingContext* FireMappingContext;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* FireAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* AimAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* Equip1Action;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* Equip2Action;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* Equip3Action;

    void Fire();

    void StartAiming();
    void StopAiming();

    void EquipWeapon1();
    void EquipWeapon2();
    void EquipWeapon3();
    

    UStaticMeshComponent* CurrentStaticMeshComponent;
    USkeletalMeshComponent* CurrentSkeletalMeshComponent;


    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UTestWidget> AmmoWidgetClass;

    UPROPERTY()
    UTestWidget* AmmoWidget;

    void UpdateAmmoDisplay();

    int32 CurrentAmmo;
    int32 MaxAmmo = 100;

private:
    bool bIsAiming;

    FVector AimOffset; // 정조준 상태에서 총의 위치 오프셋
    FVector DefaultOffset; // 기본 총 위치 오프셋
    FRotator AimRotation; // 정조준 상태에서 총의 회전
    FRotator DefaultRotation; // 기본 총 회전

    float AimingSpeed;
    void UpdateWeaponPosition();
};