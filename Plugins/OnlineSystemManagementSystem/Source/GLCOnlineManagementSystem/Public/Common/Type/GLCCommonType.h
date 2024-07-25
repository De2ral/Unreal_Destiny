#pragma once
#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "GameFramework/OnlineReplStructs.h"
#include "GameFramework/PlayerState.h"

#include "GLCCommonType.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogGLCOnlineManagementSystem, Log, All)

struct FUniqueNetIdRepl;

USTRUCT(BlueprintType)
struct GLCONLINEMANAGEMENTSYSTEM_API FOptionalParamBase
{
	GENERATED_USTRUCT_BODY()

	FOptionalParamBase()
		:AsBool(false), AsInt(INDEX_NONE), AsFlot(INDEX_NONE)
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OptionalParam")
	FString AsString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OptionalParam")
	bool	AsBool;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OptionalParam")
	int64	AsInt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OptionalParam")
	float	AsFlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OptionalParam")
	FText	AsText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OptionalParam")
	FName	AsName;
};

USTRUCT(BlueprintType)
struct GLCONLINEMANAGEMENTSYSTEM_API FOptionalParamV1 : public FOptionalParamBase
{
	GENERATED_USTRUCT_BODY()
	
	FOptionalParamV1()
	{
		AsTextures = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OptionalParam")
	UTexture2D* AsTextures;
};

USTRUCT(BlueprintType)
struct GLCONLINEMANAGEMENTSYSTEM_API FPlatformManageOptionalParam
{
	GENERATED_USTRUCT_BODY()

	FPlatformManageOptionalParam(const FName& InName)
	{
		OptionalValues.Add(InName);
	}
	FPlatformManageOptionalParam()
	{
		
	}

	static FPlatformManageOptionalParam DefauOptionalParam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OptionalParam")
	TArray<FName> OptionalValues;
};


USTRUCT(BlueprintType)
struct GLCONLINEMANAGEMENTSYSTEM_API FGLCPlayerInfo
{
	GENERATED_USTRUCT_BODY()

	FGLCPlayerInfo()
	:	bIsMute(false),
		bMicrophoneOpen(false)
	{
		
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	FString PlayerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	FUniqueNetIdRepl NetID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	bool bIsMute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	bool bMicrophoneOpen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	TWeakObjectPtr<APlayerState> PlayerState;

	bool IsValid()
	{
		return !PlayerName.IsEmpty() && NetID->IsValid();
	}

	bool friend operator==(const FGLCPlayerInfo& InLParam, const FGLCPlayerInfo& InRParam)
	{
		return InLParam.PlayerName == InRParam.PlayerName && InLParam.NetID == InRParam.NetID;
	}
};
