
#include "CPP_GreenWall.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

// Sets default values
ACPP_GreenWall::ACPP_GreenWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
    RootComponent = Sphere;

    // Set default rotation speed
    RotationSpeed = 50.0f;

    // Create and attach Cube components
    for (int32 i = 0; i < 4; i++)
    {
        UStaticMeshComponent* Cube = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("Cube%d"), i));
        Cube->SetupAttachment(Sphere);
        Cubes.Add(Cube);
    }
}

// Called when the game starts or when spawned
void ACPP_GreenWall::BeginPlay()
{
	Super::BeginPlay();

    for (int32 i = 0; i < Cubes.Num(); i++)
    {
        float Angle = FMath::DegreesToRadians(i * 90.0f); // Adjust angle for each cube
        FVector Location = FVector(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0.0f);
        Cubes[i]->SetRelativeLocation(Location);
    }
	
}

// Called every frame
void ACPP_GreenWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateSphere(DeltaTime);
}

void ACPP_GreenWall::RotateSphere(float DeltaTime)
{
	FRotator NewRotation = Sphere->GetRelativeRotation();
    NewRotation.Yaw += RotationSpeed * DeltaTime;
    Sphere->SetRelativeRotation(NewRotation);
}