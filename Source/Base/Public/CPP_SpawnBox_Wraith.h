// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Wraith.h"
#include "Components/StaticMeshComponent.h" 
#include "Components/BoxComponent.h"
#include "CPP_SpawnBox_Wraith.generated.h"


UCLASS()
class BASE_API ACPP_SpawnBox_Wraith : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_SpawnBox_Wraith();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* SpawnBox;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Spawning")
    TSubclassOf<ACPP_Wraith> EnemyClass;

    FTimerHandle SpawnTimerHandle;    
    ACPP_Wraith* SpawnedEnemy = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Spawning")
    TSubclassOf<class AAIController_Wraith> AIControllerClass;

	UFUNCTION(BlueprintCallable, Category = "Target")
    FVector GetTargetLocation_() const;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Target")
    UStaticMeshComponent* TargetMeshComponent;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    float SpawnInterval;


    void SpawnEnemy();

    void OnSpawnTimerElapsed();
    
    bool IsEnemyValid();

    void CreateTargetMesh();


};
