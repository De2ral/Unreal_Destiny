#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlatformManage/Common/PlatformManageType.h"
#include "Common/Type/GLCCommonType.h"

#include "GLCPlatformManageLibrary.generated.h"

class UGLCOnlineManageComponent;
class USaveGame;
struct FFileEncryptionData;
class APlayerController;
struct FUniqueNetIdRepl;
struct FGLCPlayerInfo;
class UGLCGameInstance;
class UGameInstance;



UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UGLCPlatformManageLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	static ANSICHAR* GetHashKey(const FString& InFileName);

	UFUNCTION(BlueprintCallable,Category = "GLCPlatformManage|Common")
	static void K2_MD5EncryptionFromFStringToMemory(const FString& InContent,const FString& InFileName,
		TArray<uint8>& OutMemory);
	static UGameInstance* GetGameInstance();

	static void K2_MD5EncryptionFromMemoryToMemory(const FString& InFileName,TArray<uint8>& OutMemory);

	static void MemoryToMultipliesOfSixteen(TArray<uint8>& OutMemory);
	static UWorld* GetWorldFormObject(UObject* InObject);

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|Common")
	static void K2_MD5EncryptionFormFStringArrayToMemory(TArray<FString> InStrings,
		const FString& InFileName,TArray<FFileEncryptionData>& OutMenmorys);

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|Common")
	static void K2_MD5DecryptFromMemoryToFString(TArray<uint8> InBytes, const FString& InFileName, FString& OutString);
	
	static void K2_MD5DecryptFromMemoryToMemory(const FString& InFileName, TArray<uint8>& OutMemory);

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|Common")
	static void K2_MD5DecryptFromMemoryToStringArray(TArray<FFileEncryptionData> InMemorys,const FString& InFileName,TArray<FString>& OutStrings);

	//Register
	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|Common")
	static bool RegisterGLCOnlineManagerComponentFromController(APlayerController* InPlayerController);

	static void PreInit();

	//Get component
	static UGLCOnlineManageComponent* GetGLCOnlineManagerComponent();

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|Common")
	static APlayerController* GetPlayerController();

	//Get net ID
	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|Common")
	static bool GetUniqueNetIDFromObject(UObject* InRequestObject, FUniqueNetIdRepl& OutNetID);

	//Get net ID
	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|Common")
	static bool GetUniqueNetID(FUniqueNetIdRepl& OutNetID);

	//Get online manager component
	static UGLCOnlineManageComponent* GetGLCOnlineManageComponentFormObject(UObject* InObject);

	UFUNCTION(BlueprintCallable,Category = "GLCPlatformManage|Common")
	static FString GetUserID();

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|Common")
	static FString GetPlatformID();

	static void GetAllPlayerInfo_OnlyPNAndNetID(TArray<FGLCPlayerInfo>& OutInfos);
	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|Common", meta = (DisplayName = "Get All Player Info"))
	static void GetAllPlayerInfo(TArray<FGLCPlayerInfo>& OutInfos,bool bGetSelfInfo = true);

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|Common", meta = (DisplayName = "Get Self Info"))
	static FGLCPlayerInfo GetSelfInfo();

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|Common", meta = (DisplayName = "Get Player Info By FString"))
	static bool GetPlayerInfoByPlayerName(const FString& InPlayerName, FGLCPlayerInfo& OutInfo);

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|Common", meta = (DisplayName = "Get Player Info By NetID"))
	static bool GetPlayerInfoByNetID(const FUniqueNetIdRepl& InNetID, FGLCPlayerInfo& OutInfo);

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|Common", meta = (DisplayName = "User ID To FString"))
	static FString UserIDToFString(const FUniqueNetIdRepl& InNetID);

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|Common", meta = (DisplayName = "Get GLC Game Instance"))
	static UGLCGameInstance* GetGLCGameInstance();

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|Common", meta = (DisplayName = "Is Player Info Equal"))
	static bool IsPlayerInfoEqual(const FGLCPlayerInfo& InLInfo, const FGLCPlayerInfo& InRInfo);

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|Common", meta = (DisplayName = "Is Net ID Equal"))
	static bool IsNetIDEqual(const FUniqueNetIdRepl& InLID, const FUniqueNetIdRepl& InRID);

};
