#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Common/Type/GLCCommonType.h"
#include "Engine/Texture2D.h"
#include "Misc\PackageName.h"

#include "MapSettings.generated.h"

class UUI_Password;
struct FOptionalParamV1;
class UTexture2D;


USTRUCT(BlueprintType)
struct GLCONLINEMANAGEMENTSYSTEM_API FMapSettingSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Setting Spec")
	FText GameMapText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Setting Spec")
	TSoftObjectPtr<UWorld> Map;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Setting Spec")
	UTexture2D* MapImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Setting Spec")
	TArray<FOptionalParamV1> OptionalParams;


	FMapSettingSpec()
	{
		MapImage = nullptr;
	}
	FString GetMapPath() const {return FPackageName::ObjectPathToPackageName(Map.ToString());}
};

USTRUCT(BlueprintType)
struct GLCONLINEMANAGEMENTSYSTEM_API FMapSettings : public FTableRowBase
{
	GENERATED_BODY()

	FMapSettings();

	UPROPERTY(EditAnywhere,Category = "MapSetting", meta = (ToolTip = "Path to the lobby map"))
	TArray<TSoftObjectPtr<UWorld>> Lobby;
	//ui for entering password
	UPROPERTY(EditAnywhere, Category = "MapSetting")
	TSubclassOf<UUI_Password> UI_Password;
	UPROPERTY(EditAnywhere, Category = "MapSetting")
	FString SaveGameSlotName;
	UPROPERTY(EditAnywhere, Category = "MapSetting")
	TArray<FMapSettingSpec> MapSettingSpecs;

	//If index equal '-1' ,it will return random map
	FString GetLobbyMapPath(int32 Index = INDEX_NONE) const;
};
