// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Kallari.h"
#include "Components/StaticMeshComponent.h" 
#include "Components/BoxComponent.h"
#include "CPP_SpawnBox_Kallari.generated.h"

UCLASS()
class BASE_API ACPP_SpawnBox_Kallari : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_SpawnBox_Kallari();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//private:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* SpawnBox;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Spawning")
    TSubclassOf<ACPP_Kallari> EnemyClass;

    FTimerHandle SpawnTimerHandle;    
    ACPP_Kallari* SpawnedEnemy = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Spawning")
    TSubclassOf<class AAIController_Kallari> AIControllerClass;

	UFUNCTION(BlueprintCallable, Category = "Target")
    FVector GetTargetLocation_() const;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Target")
    UStaticMeshComponent* TargetMeshComponent;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    float SpawnInterval;

    //FTimerHandle SpawnTimerHandle;

    void SpawnEnemy();

    void OnSpawnTimerElapsed();
    
    bool IsEnemyValid();

    void CreateTargetMesh();
};
