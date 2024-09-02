// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
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

	UPROPERTY(EditAnywhere)
	class USphereComponent* ObjCollider;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ObjMesh;

	UPROPERTY(VisibleAnywhere)
	class ADestinyFPSBase* APlayer;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDestroy();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDestroy();

	UFUNCTION(Server,Reliable)
	void ServerObjAction();

	UFUNCTION(NetMulticast,Reliable)
	void MultiCastObjAction();

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite, Category = "Item")
	int32 MinItemValue = 1;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite, Category = "Item")
	int32 MaxItemValue = 10;

	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TArray<TSubclassOf<AActor>> ItemsToSpawn;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void ItemDrop(int32 ItemCount);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,Replicated)
	float ObjInteractTime = 60.0f;

	UFUNCTION()
	void ObjAction(ADestinyFPSBase* Player);

	UFUNCTION()
	UStaticMeshComponent* GetObjMesh() { return ObjMesh; }

};
