// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "ItemComponent.h"
#include "CPP_ItemBase.generated.h"

UCLASS()
class BASE_API ACPP_ItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPP_ItemBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UItemComponent* Item;

};
