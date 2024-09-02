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

	virtual void ObjAction(ADestinyFPSBase* Player) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDestroy();

	// 리플리케이션 설정 함수
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

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

};
