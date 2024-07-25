#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlatformManage/OnlineSessions/Common/GLCOnlineSessionsType.h"

#include "GLCOnlineSessionsConfig.generated.h"

struct FGLCSessionCreateSettings;


USTRUCT(BlueprintType)
struct GLCONLINEMANAGEMENTSYSTEM_API FSessionConfigSpec
{
	GENERATED_BODY()

	UPROPERTY(Category = "SessionConfigSpec", BlueprintReadWrite,EditAnywhere)
	FGLCSessionCreateSettings SessionCreateSettings;
};

UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UGLCOnlineSessionsConfig : public USaveGame
{
	GENERATED_BODY()

public:
	UGLCOnlineSessionsConfig();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GLCOnlineSessionsConfig")
	FSessionConfigSpec OnlineSessionsConfigSpec;
};
