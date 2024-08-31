// Fill out your copyright notice in the Description page of Project Settings.


#include "Marker.h"
#include "Components/WidgetComponent.h"

// Sets default values
AMarker::AMarker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 위젯 컴포넌트 초기화
    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MarkerWidget"));
    WidgetComponent->SetupAttachment(RootComponent);

    // 위젯 컴포넌트의 공간을 월드 공간으로 설정
    WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
}

// Called when the game starts or when spawned
void AMarker::BeginPlay()
{
	Super::BeginPlay();

	DisplayMarkerWidget();
	
}

void AMarker::DisplayMarkerWidget()
{
	if (WidgetClass)
    {
        // 위젯 컴포넌트에 표시할 위젯 클래스를 설정합니다.
        WidgetComponent->SetWidgetClass(WidgetClass);

        // 위젯 생성 및 초기화
        UUserWidget* MarkerWidget = WidgetComponent->GetUserWidgetObject();
        if (MarkerWidget)
        {
            // 위젯을 활성화하거나 초기 설정을 여기서 할 수 있습니다.
            MarkerWidget->AddToViewport();
        }
    }


}

// Called every frame
void AMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 플레이어의 위치와 카메라 정보를 가져옴
    //PlayerLocation = GetPlayerCameraManager().GetCameraLocation();
    //PlayerForwardVector = GetPlayerCameraManager().GetCameraRotation().Vector();

    //// 목표 위치를 기준으로 마커 위치 설정
    //TargetLocation = [퀘스트 목표 위치];

    //// 마커의 위치를 플레이어 카메라 앞에 고정시킴
    //Vector MarkerLocation = PlayerLocation + (PlayerForwardVector * 1000.0f);
    //SetActorLocation(MarkerLocation);

}

