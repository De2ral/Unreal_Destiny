// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowAnimNotify.h"
#include "DestinyFPSBase.h"

void UThrowAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp && MeshComp->GetOwner())
    {
        ADestinyFPSBase* Player = Cast<ADestinyFPSBase>(MeshComp->GetOwner());
        if (Player)
        {
            Player->Throw();
        }
    }
}