// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "UI_LoadingMap.generated.h"

class UPackage;


UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UUI_LoadingMap : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "LoadingMap")
	TSoftObjectPtr<UWorld> Map;
	FString MapPath;

	TWeakObjectPtr<UPackage> Package;
	float CurrentPercentage;
	bool bComplete;
	bool bCall;
public:
	virtual void NativeConstruct()override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

protected:
	UFUNCTION(BlueprintNativeEvent,Category="LoadingMap")
	void OnLoadingMapProgress(float InPercentage);
	virtual void OnLoadingMapProgress_Implementation(float InPercentage);

	UFUNCTION(BlueprintNativeEvent,Category="LoadingMap")
	void OnLoadingMapComplete();
	virtual void OnLoadingMapComplete_Implementation();

	UFUNCTION(BlueprintCallable,Category="LoadingMap")
	void JoinMap();

};
