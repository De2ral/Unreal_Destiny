#pragma once

#include "CoreMinimal.h"
#include "Engine/HitResult.h"
#include "DamageInfo.generated.h"

USTRUCT()
struct FDamageInfo
{
    GENERATED_BODY()

    UPROPERTY()
    float DamageAmount;

    UPROPERTY()
    FHitResult HitResult;

    UPROPERTY()
    AActor* DamageCauser;
};