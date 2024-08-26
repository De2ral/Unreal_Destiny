// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_GreenWall.generated.h"

UCLASS()
class BASE_API ACPP_GreenWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_GreenWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	  
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    UStaticMeshComponent* Sphere;
    
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TArray<UStaticMeshComponent*> Cubes;
    
    UPROPERTY(EditAnywhere, Category = "Rotation")
    float RotationSpeed;
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Radius = 300.0f;

    void RotateSphere(float DeltaTime);
};


