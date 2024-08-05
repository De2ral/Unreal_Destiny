// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CPP_SpawnBox.generated.h"

UCLASS()
class BASE_API ACPP_SpawnBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_SpawnBox();

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
    TSubclassOf<AActor> EnemyClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Spawning")
    TSubclassOf<class AAIController_Dorongo> AIControllerClass;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    float SpawnInterval;

    FTimerHandle SpawnTimerHandle;

    void SpawnEnemy();


};
