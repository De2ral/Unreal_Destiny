// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Marker.generated.h"

UCLASS()
class BASE_API AMarker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMarker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 이 컴포넌트는 위젯을 화면에 표시하는 데 사용됩니다.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UWidgetComponent* WidgetComponent;

    // 표시할 위젯의 클래스를 설정합니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> WidgetClass;

    // 위젯을 화면에 표시하는 함수
    void DisplayMarkerWidget();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
