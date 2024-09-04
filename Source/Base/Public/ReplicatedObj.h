// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ReplicatedObj.generated.h"

UCLASS()
class BASE_API AReplicatedObj : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReplicatedObj();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* ObjCollider;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* ObjMesh;

	UPROPERTY(VisibleAnywhere)
	class ADestinyFPSBase* APlayer;

	UPROPERTY(Replicated)
	class ADestinyFPSBase* DeadPlayer;

	// UFUNCTION()
	// void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// UFUNCTION()
	// void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TArray<TSubclassOf<AActor>> ItemsToSpawn;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> QuestUIClass;

	UUserWidget* QuestUIInstance;

	bool bIsFill = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float ObjInteractTime = 60.0f;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void ItemDrop(int32 ItemCount);

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Item")
	int32 MinItemValue = 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Item")
	int32 MaxItemValue = 10;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* AfterInteractMesh;

	UFUNCTION()
	UStaticMeshComponent* GetObjMesh() { return ObjMesh; }

	UFUNCTION()
	void ShowQuestUI();

	UFUNCTION(BlueprintCallable)
	bool GetObjIsFill() {return bIsFill; }

	UFUNCTION(BlueprintCallable)
	void SetObjIsFill(bool value) { bIsFill = value; }

};
