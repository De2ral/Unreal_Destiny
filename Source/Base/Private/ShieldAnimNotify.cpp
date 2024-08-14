// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldAnimNotify.h"
#include "DestinyFPSBase.h"

void UShieldAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp && MeshComp->GetOwner())
    {
        ADestinyFPSBase* Player = Cast<ADestinyFPSBase>(MeshComp->GetOwner());
        if (Player)
        {
            Player->Shield();
        }
    }
}