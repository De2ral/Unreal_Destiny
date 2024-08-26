// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CPP_BossWall.generated.h"

UCLASS()
class BASE_API ACPP_BossWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_BossWall();

	void MoveWall();
	
    FVector MoveDirection;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
    float MoveSpeed;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBoxComponent* Collider;

};
