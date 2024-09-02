// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "InteractableObject.generated.h"

UCLASS()
class BASE_API AInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Replicated)
	class USphereComponent* ObjCollider;

	UPROPERTY(EditAnywhere,Replicated)
	class UStaticMeshComponent* ObjMesh;

	UPROPERTY(VisibleAnywhere)
	class ADestinyFPSBase* APlayer;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	 // 리플리케이션 설정 함수
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(Server,Reliable)
	void OnRepObjAction();

	UFUNCTION(NetMulticast,Reliable)
	void MultiCastObjAction();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,Replicated)
	float ObjInteractTime = 0.0f;

	UFUNCTION()
	virtual void ObjAction(ADestinyFPSBase* Player) {};

	UFUNCTION()
	UStaticMeshComponent* GetObjMesh() { return ObjMesh; }

};
