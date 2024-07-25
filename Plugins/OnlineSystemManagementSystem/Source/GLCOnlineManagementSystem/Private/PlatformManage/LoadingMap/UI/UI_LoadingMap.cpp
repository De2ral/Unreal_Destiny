// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformManage/LoadingMap/UI/UI_LoadingMap.h"
#include <Kismet/GameplayStatics.h>
#include "UObject\Package.h"


void UUI_LoadingMap::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentPercentage = 0.f;
	bComplete = false;
	bCall = false;

	if(GetOwningPlayer())
	{
		GetOwningPlayer()->DisableInput(GetOwningPlayer());
	}
	MapPath = FString(FPackageName::ObjectPathToPackageName(Map.ToString()));
	checkf(!MapPath.IsEmpty(), TEXT("Ensure that the map path to load is not empty"));
	if (!MapPath.IsEmpty())
	{
		FLoadPackageAsyncDelegate MyDelegate;
		MyDelegate.BindLambda(
			[&](const FName& InPackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
			{
				if(Result == EAsyncLoadingResult::Succeeded)
				{
					Package = LoadedPackage;
					bComplete = true;
				}
			});
		LoadPackageAsync(MapPath, MyDelegate, 0,
			EPackageFlags::PKG_ContainsMap);
	}
}

void UUI_LoadingMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);

	if (!MapPath.IsEmpty() && !bComplete && !bCall)
	{
		float TempV = GetAsyncLoadPercentage(*MapPath);
		if (TempV != -1)
		{
			CurrentPercentage = TempV;
			CurrentPercentage /= 100.f;
			OnLoadingMapProgress(CurrentPercentage);
		}
	}

	if (bComplete && CurrentPercentage < 1)
	{
		CurrentPercentage += InDeltaTime * 10.f;
		CurrentPercentage = FMath::Min(1.f,CurrentPercentage);
		OnLoadingMapProgress(CurrentPercentage);
		if (CurrentPercentage >= 1)
		{
			bCall = true;
			OnLoadingMapComplete();
		}
	}
}

void UUI_LoadingMap::OnLoadingMapProgress_Implementation(float InPercentage)
{
	
}

void UUI_LoadingMap::OnLoadingMapComplete_Implementation()
{
	
}

void UUI_LoadingMap::JoinMap()
{
	if (Package.IsValid())
	{
		UGameplayStatics::OpenLevel(GetWorld(),*MapPath);
	}
}
