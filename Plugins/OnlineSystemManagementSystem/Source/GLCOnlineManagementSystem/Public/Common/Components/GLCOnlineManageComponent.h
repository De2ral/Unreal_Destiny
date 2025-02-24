#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Common/Type/GLCCommonType.h"
#include "PlatformManage/Common/GLCPlatformObjectBase.h"
#include "PlatformManage/OnlineSessions/Common/GLCOnlineSessionsType.h"

#include "GLCOnlineManageComponent.generated.h"


class UGLCUserCloundObject;
class UGLCOnlineSessionObject;
class UUI_MenuMain;
struct FGLCPlayerInfo;
struct FUniqueNetIdRepl;
class APlayerState;
class UGLCOnlineVoiceObject;
class UUI_VoiceChatMain;


UCLASS(Blueprintable)
class GLCONLINEMANAGEMENTSYSTEM_API UGLCOnlineSessionObject : public UGLCPlatformObjectBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "OnlineSession")
	void OnCreateSessionComplete(bool bWasSuccessful, const FPlatformManageOptionalParam& OptionalParam);

	UFUNCTION(BlueprintNativeEvent, Category = "OnlineSession")
	void OnFindSessionComplete(const TArray<FGLCOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful, const FPlatformManageOptionalParam& OptionalParam);

	UFUNCTION(BlueprintNativeEvent, Category = "OnlineSession")
	void OnJoinSessionComplete(EjoinSessionCompleteType JoinType, const FPlatformManageOptionalParam& OptionalParam);

	UFUNCTION(BlueprintNativeEvent, Category = "OnlineSession")
	void OnDestroySessionComplete(bool bWasSuccessful, const FPlatformManageOptionalParam& OptionalParam);

	UFUNCTION(BlueprintNativeEvent, Category = "OnlineSession")
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetIdRepl& UserId, const FString& Error,const FPlatformManageOptionalParam& OptionalParam);

	UFUNCTION(BlueprintNativeEvent, Category = "OnlineSession")
	void OnLogStatusChanged(int32 LocalUserNum, EGLCLoginState OldStatus, EGLCLoginState NewStatus, const FUniqueNetIdRepl& NewId);

};

UCLASS(Blueprintable)
class GLCONLINEMANAGEMENTSYSTEM_API UGLCUserCloundObject : public UGLCPlatformObjectBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "UserClound")
		void OnReadUserFileComplete(bool bSuccessful, const FUniqueNetIdRepl& NetID, const FString& FileName,
			const FPlatformManageOptionalParam& OptionalParam);

	UFUNCTION(BlueprintNativeEvent, Category = "UserClound")
		void OnWriteUserFileInProgress(int32 CurrentWriteBytes, const FUniqueNetIdRepl& NetID, const FString& FileName, const FPlatformManageOptionalParam& OptionalParam);

	UFUNCTION(BlueprintNativeEvent, Category = "UserClound")
		void OnWriteUserFileComplete(bool bSuccessful, const FUniqueNetIdRepl& InNetID, const FString& InFileName, const FPlatformManageOptionalParam& OptionalParam);

	UFUNCTION(BlueprintNativeEvent, Category = "UserClound")
		void OnDeleteUserFileComplete(bool bSuccessful, const FUniqueNetIdRepl& InNetID, const FString& InFileName, const FPlatformManageOptionalParam& OptionalParam);

	UFUNCTION(BlueprintNativeEvent, Category = "UserClound")
		void OnEnumerateUserFileComplete(bool bSuccessful, const FUniqueNetIdRepl& InNetID, const FPlatformManageOptionalParam& OptionalParam);

};



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GLCONLINEMANAGEMENTSYSTEM_API UGLCOnlineManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGLCOnlineManageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "GLCOnlineManageComponentCoreSettings|OnlineSession")
		TArray<TSubclassOf<UGLCOnlineSessionObject>> GLCOnlineSessionObjectClasses;
	UPROPERTY(BlueprintReadWrite, Category = "GLCOnlineManageComponentCoreSettings|OnlineSession")
		UUI_MenuMain* MenuMain;

	UPROPERTY(EditAnywhere, Category = "GLCOnlineManageComponentCoreSettings|UserClounds")
		TArray<TSubclassOf<UGLCUserCloundObject>> GLCUserCloundObjectClasses;

	UPROPERTY(EditAnywhere, Category = "GLCOnlineManageComponentCoreSettings|Voice")
	TSubclassOf<UGLCOnlineVoiceObject> GLCOnlineVoiceChatObject;
protected:
	UPROPERTY()
	UUI_VoiceChatMain* VoiceChatMain;
public:
	UUI_VoiceChatMain* GetVoiceChatMain();
	void SetVoiceChatMain(UUI_VoiceChatMain* val);
public:
	UFUNCTION(Server, Reliable)
	void Server_StartGame();
	UFUNCTION(Server, Reliable)
	void Server_StartGameByKey(const int32 InKey);

	UFUNCTION(Server, Reliable)
	void Server_OnPlayerTalkingStateChange(const FUniqueNetIdRepl& InNetID, bool bMicrophoneOpen);
	UFUNCTION(Client, Reliable)
	void Client_OnPlayerTalkingStateChange(const FUniqueNetIdRepl& InNetID, bool bIsTbMicrophoneOpenalking);



};
