#pragma once
#include "CoreMinimal.h"

#include "GLCPlatformObjectBase.generated.h"


UCLASS(Blueprintable)
class GLCONLINEMANAGEMENTSYSTEM_API UGLCPlatformObjectBase : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable,Category = "GLCPlatformObjectBase")
	AActor* GetObjectOwner();

};
