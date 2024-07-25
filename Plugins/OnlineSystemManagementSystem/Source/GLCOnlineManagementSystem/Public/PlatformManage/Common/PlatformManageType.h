#pragma once
#include "CoreMinimal.h"

#include "PlatformManageType.generated.h"

class UGLCOnlineManageComponent;
struct FUniqueNetIdRepl;
class APlayerController;


struct GLCONLINEMANAGEMENTSYSTEM_API FOnlineSubsystemDataManage
{
private:
	TWeakObjectPtr<UGLCOnlineManageComponent> OnlineManageComponent;
	TWeakObjectPtr<APlayerController>		  MyPlayerController___;

	static FOnlineSubsystemDataManage* OnlineSubsystemDataManage;

public:
	FOnlineSubsystemDataManage();

	//Basic
	static FOnlineSubsystemDataManage* Get();
	static void Destroy();
	static bool GetUniqueID(FUniqueNetIdRepl& OutNetID);

	UGLCOnlineManageComponent* GetOnlineManageComponent();
	void SetOnlineManageComponent(UGLCOnlineManageComponent* val);

	APlayerController* GetMyPlayerController();
	void SetMyPlayerController(APlayerController* val);
};

USTRUCT(BlueprintType)
struct GLCONLINEMANAGEMENTSYSTEM_API FFileEncryptionData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "FileData")
	TArray<uint8> SingleDatas;

};
