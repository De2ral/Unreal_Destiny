#pragma once
#include "CoreMinimal.h"
#include "Common/Type/GLCCommonType.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/Texture2D.h"

#include "GLCOnlineSessionsType.generated.h"

class UGLCOnlineSessionsConfig;
class UGLCOnlineManageComponent;


namespace OnlineSessionStaticDef
{
	static const FString DataTablePath = TEXT("/GLCOnlineManagementSystem/ManagementSystem/DataTable/Table_MapSettings");

	static const FString MenuMainPath = TEXT("/GLCOnlineManagementSystem/ManagementSystem/MenuBP/Core/BP_UI_MenuMain");

	static const FString LobbyStartPath = TEXT("/GLCOnlineManagementSystem/ManagementSystem/MenuBP/Lobby/BP_LobbyStart");

	static const FString MapConfigKey = TEXT("MapConfig");
	static const FString RoomNameKey  = TEXT("RoomName");
	static const FString RoomPassword = TEXT("RoomPassword");
	static const FString bPassword = TEXT("bPassword");
	static const FString OnCreateSessionCompleteKey		=			TEXT("OnCreateSessionCompleteKey");
	static const FString OnFindSessionCompleteKey		=			TEXT("OnFindSessionCompleteKey");
	static const FString OnJoinSessionCompleteKey		=			TEXT("OnJoinSessionCompleteKey");
	static const FString OnDestroySessionCompleteKey	=			TEXT("OnDestroySessionCompleteKey");
	static const FString OnLoginCompleteKey				=			TEXT("OnLoginCompleteKey");
}

UENUM(BlueprintType)
enum class EGLCLoginState : uint8
{
	/** Player has not logged in or chosen a local profile */
	NotLoggedIn,
	/** Player is using a local profile but is not logged in */
	UsingLocalProfile,
	/** Player has been validated by the platform specific authentication service */
	LoggedIn
};

UENUM(BlueprintType)
enum class EJoinSessionRequestState : uint8
{
	JOIN_SESSION_REQUEST_FAIL = 0				UMETA(DisplayName = "Join session request fail"),
	ALREADY_IN_SESSION_DESTROYING_SESSION_AND_REJOIN = 1	UMETA(DisplayName = "Already in session,destroying session and rejoin"),
	JOIN_SESSION_REQUEST_SUCCESSFUL = 2			UMETA(DisplayName = "Join session request successful"),
	Waiting_Password = 3							UMETA(DisplayName = "Waiting Password"),
};

UENUM(BlueprintType)
enum class EjoinSessionCompleteType : uint8
{
	/** The join worked as expected */
	Success								UMETA(DisplayName = "Success"),
	/** There are no open slots to join */
	SessionIsFull						UMETA(DisplayName = "SessionIsFull"),
	/** The session couldn't be found on the service */
	SessionDoesNotExist					UMETA(DisplayName = "Session Does Not Exist"),
	/** There was an error getting the session server's address */
	CouldNotRetrieveAddress				UMETA(DisplayName = "Could Not Retrieve Address"),
	/** The user attempting to join is already a member of the session */
	AlreadyInSession					UMETA(DisplayName = "Already In Session"),
	/** An error not covered above occurred */
	UnknownError						UMETA(DisplayName = "Unknown Error")
};

USTRUCT(BlueprintType)
struct GLCONLINEMANAGEMENTSYSTEM_API FGLCSessionOptionalParam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OptionalParam")
	TMap<FName, FString> SettingSpecs;

};

USTRUCT(BlueprintType)
struct GLCONLINEMANAGEMENTSYSTEM_API FGLCSessionCreateSettings
{
	GENERATED_BODY()

		FGLCSessionCreateSettings()
	{
		NumPublicConnections = 16;
		NumPrivateConnections = 0;
		bShouldAdvertise = true;
		bAllowJoinInProgress = true;
		bIsDedicated = false;
		bUsesStats = true;
		bAllowInvites = true;
		bUsesPresence = true;
		bAllowJoinViaPresence = true;
		bAllowJoinViaPresenceFriendsOnly = false;
		bAntiCheatProtected = false;
		bUseLobbiesIfAvailable = true;
		bUseLobbiesVoiceChatIfAvailable = true;
		BuildUniqueId = 1;

		MapID = 0;
		RoomName = TEXT("MyRoom");
		bPassword = false;
	}
	//Necessary settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting|Necessary")
	int32 MapID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting|Necessary")
	FString RoomName;
	//Room password
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearchResult")
	FString RoomPassword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearchResult")
	bool bPassword;
	//
	/** The number of publicly available connections advertised */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting|Necessary", meta = (ToolTip = "The number of publicly available connections advertised"))
	int32 NumPublicConnections;
	/** Whether joining in progress is allowed or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting", meta = (ToolTip = "Whether joining in progress is allowed or not"))
	bool bAllowJoinInProgress;

	/** The number of connections that are private (invite/password) only */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting")
		int32 NumPrivateConnections;
	/** Whether this match is publicly advertised on the online service */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting")
		bool bShouldAdvertise;
	/** Whether the server is dedicated or player hosted */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting")
		bool bIsDedicated;
	/** Whether the match should gather stats or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting")
		bool bUsesStats;
	/** Whether the match allows invitations for this session or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting")
		bool bAllowInvites;
	/** Whether to display user presence information or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting")
		bool bUsesPresence;
	/** Whether joining via player presence is allowed or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting", meta = (ToolTip = "Whether joining via player presence is allowed or not"))
	bool bAllowJoinViaPresence;
	/** Whether joining via player presence is allowed for friends only or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting", meta = (ToolTip = "Whether joining via player presence is allowed for friends only or not"))
	bool bAllowJoinViaPresenceFriendsOnly;
	/** Whether the server employs anti-cheat (punkbuster, vac, etc) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting", meta = (ToolTip = "Whether the server employs anti-cheat (punkbuster, vac, etc)"))
	bool bAntiCheatProtected;
	/** Whether to prefer lobbies APIs if the platform supports them */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting", meta = (ToolTip = "Whether to prefer lobbies APIs if the platform supports them"))
	bool bUseLobbiesIfAvailable;
	/** Whether to create (and auto join) a voice chat room for the lobby, if the platform supports it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting", meta = (ToolTip = "Whether to create (and auto join) a voice chat room for the lobby, if the platform supports it"))
	bool bUseLobbiesVoiceChatIfAvailable;
	/** Manual override for the Session Id instead of having one assigned by the backend. Its size may be restricted depending on the platform */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting", meta = (ToolTip = "Manual override for the Session Id instead of having one assigned by the backend. Its size may be restricted depending on the platform"))
	FString SessionIdOverride;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting|Optional")
	FGLCSessionOptionalParam OptionalParam;

	/** Used to keep different builds from seeing each other during searches */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionCreateSetting", meta = (ToolTip = "Used to keep different builds from seeing each other during searches"))
		int32 BuildUniqueId;
};

USTRUCT(BlueprintType)
struct GLCONLINEMANAGEMENTSYSTEM_API FGLCOnlineSessionSearchResult
{
	GENERATED_BODY()

	FGLCOnlineSessionSearchResult()
	{
		MaxPlayer = INDEX_NONE;
		CurrentPlayer = INDEX_NONE;
		NumPrivateOpenConnections = INDEX_NONE;
		NumPrivateConnections = INDEX_NONE;
		Ping = INDEX_NONE;
		MapID = INDEX_NONE;
		bPassword = false;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearchResult")
	int32 MaxPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearchResult")
	int32 CurrentPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearchResult")
	int32 NumPrivateOpenConnections;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearchResult")
	int32 NumPrivateConnections;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearchResult")
	FString OwningUserName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearchResult")
	int32 Ping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearchResult")
	FString RoomName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearchResult")
	int32 MapID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearchResult")
	FString RoomPassword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearchResult")
	bool bPassword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearchResult")
	FGLCSessionOptionalParam Settings;

	TSharedPtr<class FOnlineSessionInfo> SessionInfo;
};

UENUM(BlueprintType)
enum class ECreateSessionRequestState : uint8
{
	CREATE_SESSTION_REQUEST_FAIL										UMETA(DisplayName = "Create session request fail"),
	ALREADY_IN_SESSION_DESTROYING_SESSION_AND_RECREATING				UMETA(DisplayName = "Already in session,destroying session and recreating"),
	/*NOT_LOGGED_IN														UMETA(DisplayName = "Not Logged In"),*/
	CREATE_SESSTION_REQUEST_SUCCESSFUL									UMETA(DisplayName = "Create sesstion request successful"),
};

UENUM(BlueprintType)
enum class EUserCountry : uint8
{
	China					UMETA(DisplayName = "China"),
	Japan					UMETA(DisplayName = "Japan"),
	USA						UMETA(DisplayName = "USA"),
	South_Korea				UMETA(DisplayName = "South Korea"),
};

UENUM(BlueprintType)
enum class ELoginMethod : uint8
{
	ExchangeCode,
	AccountAndPassword,
	AccountPortal,
	PersistentAuth
};