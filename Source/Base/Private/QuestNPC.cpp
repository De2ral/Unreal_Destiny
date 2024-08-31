// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestNPC.h"
#include "DestinyFPSBase.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AQuestNPC::AQuestNPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AQuestNPC::BeginPlay()
{
	Super::BeginPlay();

	if (QuestUIClass)
	{
		QuestUIInstance = CreateWidget<UUserWidget>(GetWorld(), QuestUIClass);
	}
	
}

// Called every frame
void AQuestNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (APlayer && APlayer->bIsInteractComplete)
	{
		ObjAction(APlayer);
		APlayer->bIsInteractComplete = false;
	}

}

void AQuestNPC::ObjAction(ADestinyFPSBase* Player)
{
	// 퀘스트 UI를 띄우는 로직
	ShowQuestUI();
}

void AQuestNPC::ShowQuestUI()
{
	if (QuestUIInstance && !QuestUIInstance->IsInViewport())
	{
		QuestUIInstance->AddToViewport();
	}
}

void AQuestNPC::HideQuestUI()
{
	if (QuestUIInstance && QuestUIInstance->IsInViewport())
	{
		QuestUIInstance->RemoveFromViewport();
	}
}
