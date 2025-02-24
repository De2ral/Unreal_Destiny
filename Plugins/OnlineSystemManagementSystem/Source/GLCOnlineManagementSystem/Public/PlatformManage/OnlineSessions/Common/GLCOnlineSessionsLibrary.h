#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlatformManage/OnlineSessions/OnlineSessionsManage.h"
#include "PlatformManage/OnlineSessions/DataTable/MapSettings.h"

#include "GLCOnlineSessionsLibrary.generated.h"

struct FMapSettingSpec;
class UOnlineSessionsManage;
class UUI_MenuMain;
class UDataTable;
struct FMapSettings;
class UUI_ConfigSet;
class UGLCOnlineManageComponent;
class AGameModeBase;
struct FMapSettings;
class UComboBoxString;
class FOnlineSessionSearchResult;
struct FGLCOnlineSessionSearchResult;
class UGLCOnlineSessionsConfig;
struct FPlatformManageOptionalParam;
class APlayerController;
class UUserWidget;
class UWorld;
class UObject;
enum class EjoinSessionCompleteType : uint8;
enum class EJoinSessionRequestState : uint8;


/**
 * 
 */
UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UGLCOnlineSessionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UGLCOnlineSessionsLibrary();
	static TSubclassOf<UUI_MenuMain> MenuMainClass;
	static const UDataTable* TablePtr;
	static TSubclassOf<UUserWidget> LobbyStartClass;
	//Acquire subsystem
	static UOnlineSessionsManage* GetGLCOnlineSessionsSubsystem();
	
	//Get World
	static UWorld* GetWorldFromObject(UObject* InObject);

	//Get Join Type
	static EjoinSessionCompleteType GetJoinSessionCompleteType(EOnJoinSessionCompleteResult::Type InType);

	//The default search results are converted to the types supported by the blueprint
	static void ConvertSearchResult(const TArray<FOnlineSessionSearchResult>& InResult,
		TArray<FGLCOnlineSessionSearchResult>& OutResults);
	static void ConvertSearchResult(const TArray<FGLCOnlineSessionSearchResult>& InResult,
		TArray<FOnlineSessionSearchResult>& OutResultst);
	static const FOnlineSessionSearchResult* ConvertSearchResult(const FGLCOnlineSessionSearchResult& InResult);

	//Convert create session setting
	static void ConvertCreateSessionSetting(const FGLCSessionCreateSettings& InCreateSetting,
		FOnlineSessionSettings& OutSettings);
	

	//Get Conference Components
	static UGLCOnlineManageComponent* GetGLCOnlineManagerComponentFormObject(UObject* InObject);

	//Get component from cache
	static UGLCOnlineManageComponent* GetGLCOnlineManagerComponent();

	//Get GLCOnlineSessionsSubsystem
	static UOnlineSessionsManage* GetGLCOnlineSessionsSubsystemFromObject(UObject* InObject);

	//Find map settings by ID
	static FMapSettingSpec* GetSettingSpecByKey(const int32 InKey);
	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary|MapSettings")
	static bool K2_GetMapSettingSpecByKey(const int32 InKey, FMapSettingSpec& OutSpec);

	//Get a row of data
	static FMapSettings* GetMapSettings();
	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary|MapSettings")
	static void GetMapSettingsSpecs(TArray<FMapSettingSpec>& OutSettingSpecs);

	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary|MapSettings")
	static void InitComboBox(UComboBoxString* InComboBox);
	
	//Find lobby path
	//If index equal '-1' ,it will return random map
	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary|MapSettings")
	static bool K2_GetPathToLobby(FString& OutLobbyPath, bool bAddListen = true ,int32 LobbyIndex = -1);
	//If index equal '-1' ,it will return random map
	static FString GetPathToLobby(bool bAddListen = true,int32 LobbyIndex = INDEX_NONE);

	//Get the configuration saved by the session
	//Note: The local session configuration will be loaded automatically

	UFUNCTION(BlueprintCallable, Category = "GLC Online Sessions Library|Map Settings")
	static UGLCOnlineSessionsConfig* GetOnlineSessionsConfigFromSlot(const int32 InUserID = 0);

	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary|MapSettings")
	static void SaveOnlineSessionsConfigToSlot(UGLCOnlineSessionsConfig* InSessionsConfig,const int32 InUserID = 0);

	//Get save game slot name from data table
	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary|MapSettings")
	static const FString GetOnlineSessionsConfigSaveGameSlotName();

	//Create Menu
	UFUNCTION(BlueprintCallable,Category = "GLCOnlineSessionsLibrary")
	static UUI_MenuMain* CreateMenu(APlayerController* InPlayerController,bool bAddToViewPort = true,bool bInputUIOnly = true, int32 ZOrder = 0);
	
	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary")
	static UUI_MenuMain* GetMenu();

	//InSettings can be empty
	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary")
	static ECreateSessionRequestState CreateSession(const FGLCSessionCreateSettings& InSettings,
		const FPlatformManageOptionalParam& InOptionParam);

	//Find the session
	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary")
	static bool FindSession(const FPlatformManageOptionalParam& InOptionParam,int32 MaxSearchResults = 100000000);

	static EJoinSessionRequestState JoinSession(const FOnlineSessionSearchResult& SessionResult,
		const FPlatformManageOptionalParam& InOptionParam);

	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary",meta=(DisplayName = "Join Session"))
	static EJoinSessionRequestState K2_JoinSession(const FGLCOnlineSessionSearchResult& SessionResult,
		const FPlatformManageOptionalParam& InOptionParam);

	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary",meta=(DisplayName = "Destroy Session"))
	static bool DestroySession(const FPlatformManageOptionalParam& InOptionParam);

	//After successfully joining the session, the client will join the living room map call
	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary")
	static bool ClientJoinMap(APlayerController* InPlayerController);

	//Called by the owner
	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary")
	static bool ServerJoinLobbyMap(APlayerController* InPlayerController,
		int32 LobbyIndex = -1, bool bRandomLobbyMap = true);

	//The server directly adds to the game map without entering the hall
	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary")
	static bool ServerJoinGameMap(APlayerController* InPlayerController);

	//To start the game in the lobby
	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary")
	static bool StartGame(APlayerController* InPlayerController, bool bCheckHasAuthority = true);

	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary")
	static bool StartGameByKey(UObject* InObject,int32 InKey ,bool bCheckHasAuthority = true);

	//Get the path of the game map
	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary")
	static bool GetPathToGameMap(FString& OutPath, bool bAddListen = true);

	//Create controls for the lobby to start the game
	//If pass a NULL widget ,it will create default widget
	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary")
	static UUserWidget* CreateLobbyWidget(TSubclassOf<UUserWidget> InWidget,APlayerController* InPlayerController,bool bInputUIOnly = true,
		bool bShowMouse = true,bool bCheckHasAuthority = true,int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary")
	static bool TryLogin(FString Account,FString Password,ELoginMethod LoginMethod, FPlatformManageOptionalParam InParam);

	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary")
	static bool Logout();

	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary")
	static bool IsLogin();

	UFUNCTION(BlueprintCallable, Category = "GLCOnlineSessionsLibrary")
	static  bool IsInSession();

};
