// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.h"
#include "MyStash.generated.h"

class UPrimitiveComponent;

UCLASS()
class BASE_API AMyStash : public AInteractableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyStash();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY(EditAnywhere)
	//class UStaticMeshComponent* StashMesh;
//
//
	//UPROPERTY(EditAnywhere,BlueprintReadWrite)
	//class USphereComponent* Collider;

	virtual void ObjAction(ADestinyFPSBase* Player);

	//UFUNCTION()
	//void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	// void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Item")
	int32 MinItemValue = 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Item")
	int32 MaxItemValue = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TArray<TSubclassOf<AActor>> ItemsToSpawn;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void ItemDrop(int32 ItemCount);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
