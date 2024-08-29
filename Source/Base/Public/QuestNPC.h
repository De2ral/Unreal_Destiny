// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "Blueprint/UserWidget.h"
#include "QuestNPC.generated.h"

UCLASS()
class BASE_API AQuestNPC : public AInteractableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> QuestUIClass;

	UUserWidget* QuestUIInstance;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ObjAction(ADestinyFPSBase* Player) override;

	void ShowQuestUI();
	void HideQuestUI();

};
