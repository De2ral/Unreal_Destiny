#include "PlatformManage/OnlineSessions/Common/GLCOnlineSessionsLibrary.h"

#include "PlatformManage/OnlineSessions/UI/Core/UI_MenuMain.h"
#include "GameFramework/PlayerController.h"
#include <Blueprint/WidgetBlueprintLibrary.h>
#include "Common/Components/GLCOnlineManageComponent.h"
#include "Components/ComboBoxString.h"
#include <PlatformManage/Common/GLCPlatformManageLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "PlatformManage/OnlineSessions/SaveGame/GLCOnlineSessionsConfig.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "OnlineSubsystem.h"
#include "Common/GLCGameInstance.h"
#include "PlatformManage/OnlineSessions/UI/UI_Password.h"


TSubclassOf<UUI_MenuMain> UGLCOnlineSessionsLibrary::MenuMainClass = nullptr;
const UDataTable* UGLCOnlineSessionsLibrary::TablePtr = nullptr;
TSubclassOf<UUserWidget> UGLCOnlineSessionsLibrary::LobbyStartClass = nullptr;


UGLCOnlineSessionsLibrary::UGLCOnlineSessionsLibrary()
{
	ConstructorHelpers::FClassFinder<UUI_MenuMain> MenuMainFinder(*OnlineSessionStaticDef::MenuMainPath);
	MenuMainClass = MenuMainFinder.Class;

	ConstructorHelpers::FObjectFinder<UDataTable>TableFinder(*OnlineSessionStaticDef::DataTablePath);
	TablePtr = Cast<UDataTable>(TableFinder.Object);

	ConstructorHelpers::FClassFinder<UUserWidget> LobbyStartFinder(*OnlineSessionStaticDef::LobbyStartPath);
	 LobbyStartClass = LobbyStartFinder.Class;
}

UUI_MenuMain* UGLCOnlineSessionsLibrary::CreateMenu(APlayerController* InPlayerController, 
	bool bAddToViewPort, bool bInputUIOnly, int32 ZOrder)
{
	if (MenuMainClass && InPlayerController && InPlayerController->IsLocalController())
	{
		if (UUI_MenuMain* InMain = CreateWidget<UUI_MenuMain>(InPlayerController, MenuMainClass))
		{
			if (bAddToViewPort)
			{
				InMain->AddToViewport(ZOrder);
			}
			if (bInputUIOnly)
			{
				UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(InPlayerController);
			}

			if (UGLCOnlineManageComponent* InManageCom = UGLCPlatformManageLibrary::GetGLCOnlineManagerComponent())
			{
				InManageCom->MenuMain = InMain;
			}
			return InMain;
		}
	}
	return nullptr;
}

UUI_MenuMain* UGLCOnlineSessionsLibrary::GetMenu()
{
	if (UGLCOnlineManageComponent* InManageCom = UGLCPlatformManageLibrary::GetGLCOnlineManagerComponent())
	{
		return InManageCom->MenuMain;
	}
	return nullptr;
}

UOnlineSessionsManage* UGLCOnlineSessionsLibrary::GetGLCOnlineSessionsSubsystem()
{
	return UOnlineSessionsManage::Get();
}

UWorld* UGLCOnlineSessionsLibrary::GetWorldFromObject(UObject* InObject)
{
	if (InObject && GEngine)
	{
		return GEngine->GetWorldFromContextObject(InObject,EGetWorldErrorMode::LogAndReturnNull);
	}
	return nullptr;
}

UOnlineSessionsManage* UGLCOnlineSessionsLibrary::GetGLCOnlineSessionsSubsystemFromObject(UObject* InObject)
{
	if (IsValid(InObject))
	{
		if (UWorld* InWorld = UGLCOnlineSessionsLibrary::GetWorldFromObject(InObject))
		{
			if (UGameInstance* InGameInstance = InWorld->GetGameInstance())
			{
				return InGameInstance->GetSubsystem<UOnlineSessionsManage>();
			}
		}
	}
	return nullptr;
}

FMapSettingSpec* UGLCOnlineSessionsLibrary::GetSettingSpecByKey(const int32 InKey)
{
	if(TablePtr)
	{
		if (FMapSettings* InSetting = GetMapSettings())
		{
			if (InSetting->MapSettingSpecs.IsValidIndex(InKey))
			{
				return &InSetting->MapSettingSpecs[InKey];
			}
			else
			{
				UE_LOG(LogGLCOnlineManagementSystem,Error,TEXT("GetSettingSpecByKey pass a invalid key !"));
				UE_LOG(LogGLCOnlineManagementSystem, Error, TEXT("GetSettingSpecByKey pass a invalid key !"));
				UE_LOG(LogGLCOnlineManagementSystem, Error, TEXT("GetSettingSpecByKey pass a invalid key !"));
				UE_LOG(LogGLCOnlineManagementSystem, Error, TEXT("GetSettingSpecByKey pass a invalid key !"));
			}
		}

	}
	
	return nullptr;
}

FMapSettings* UGLCOnlineSessionsLibrary::GetMapSettings()
{
	TArray<FMapSettings*> InSettings;
	TablePtr->GetAllRows(TEXT("MapConfig"), InSettings);

	for (auto& MainTem : InSettings)
	{
		return MainTem;
	}
	return nullptr;
}

void UGLCOnlineSessionsLibrary::GetMapSettingsSpecs(TArray<FMapSettingSpec>& OutSettingSpecs)
{
	if (GetMapSettings())
	{
		OutSettingSpecs = GetMapSettings()->MapSettingSpecs;
	}
}

const FString UGLCOnlineSessionsLibrary::GetOnlineSessionsConfigSaveGameSlotName()
{
	if (FMapSettings* InMapSettings = GetMapSettings())
	{
		return InMapSettings->SaveGameSlotName;
	}
	return FString();
}

void UGLCOnlineSessionsLibrary::InitComboBox(UComboBoxString* InComboBox)
{
	if (InComboBox)
	{
		if (FMapSettings* InSetting = GetMapSettings())
		{
			InComboBox->ClearOptions();

			for (auto& Tem : InSetting->MapSettingSpecs)
			{
				InComboBox->AddOption(Tem.GameMapText.ToString());
			}
		}
		if (UGLCOnlineSessionsConfig* InConfig = GetOnlineSessionsConfigFromSlot())
		{
			InComboBox->SetSelectedIndex(InConfig->OnlineSessionsConfigSpec.SessionCreateSettings.MapID);
		}
	}
}

bool UGLCOnlineSessionsLibrary::K2_GetMapSettingSpecByKey(const int32 InKey, FMapSettingSpec& OutSpec)
{
	if(FMapSettingSpec* InSpec = GetSettingSpecByKey(InKey))
	{
		OutSpec = *InSpec;
		return true;
	}
	return false;
}

bool UGLCOnlineSessionsLibrary::K2_GetPathToLobby(FString& OutLobbyPath, bool bAddListen ,
	int32 LobbyIndex)
{
	FString InLobbyPath = GetPathToLobby(bAddListen,LobbyIndex);
	if (!InLobbyPath.IsEmpty())
	{
		OutLobbyPath = *InLobbyPath;
		return true;
	}
	return false;
}

FString UGLCOnlineSessionsLibrary::GetPathToLobby(bool bAddListen,int32 LobbyIndex)
{
	if (FMapSettings* InSetting = GetMapSettings())
	{
		FString LobbyPath = InSetting->GetLobbyMapPath(LobbyIndex);
		if (bAddListen && !LobbyPath.IsEmpty())
		{
			LobbyPath += TEXT("?listen");
		}
		return LobbyPath;
	}
	return FString();
}

UGLCOnlineSessionsConfig* UGLCOnlineSessionsLibrary::GetOnlineSessionsConfigFromSlot(const int32 InUserID)
{
	if (UGameplayStatics::DoesSaveGameExist(GetOnlineSessionsConfigSaveGameSlotName(),InUserID))
	{	
		return Cast<UGLCOnlineSessionsConfig>(UGameplayStatics::LoadGameFromSlot(GetOnlineSessionsConfigSaveGameSlotName(), InUserID));
	}
	else
	{
		return Cast<UGLCOnlineSessionsConfig>(UGameplayStatics::CreateSaveGameObject(UGLCOnlineSessionsConfig::StaticClass()));
	}
	return nullptr;
}

void UGLCOnlineSessionsLibrary::SaveOnlineSessionsConfigToSlot(UGLCOnlineSessionsConfig* InSessionsConfig,const int32 InUserID)
{
	UGameplayStatics::SaveGameToSlot(InSessionsConfig,GetOnlineSessionsConfigSaveGameSlotName(),InUserID);
}

ECreateSessionRequestState UGLCOnlineSessionsLibrary::CreateSession(const FGLCSessionCreateSettings& InSettings
	, const FPlatformManageOptionalParam& InOptionParam /*= FPlatformManageOptionalParam()*/)
{
	if(UOnlineSessionsManage* InSubSystem = GetGLCOnlineSessionsSubsystem())
	{
		return InSubSystem->CreateSession_(InSettings, InOptionParam);
	}
	return ECreateSessionRequestState::CREATE_SESSTION_REQUEST_FAIL;
}

bool UGLCOnlineSessionsLibrary::FindSession(const FPlatformManageOptionalParam& InOptionParam, int32 MaxSearchResults /*= 100000000*/)
{
	if (UOnlineSessionsManage* InSubSystem = GetGLCOnlineSessionsSubsystem())
	{
		return InSubSystem->FindSessions(MaxSearchResults, InOptionParam);
	}
	return false;
}

EJoinSessionRequestState UGLCOnlineSessionsLibrary::JoinSession(const FOnlineSessionSearchResult& SessionResult
	, const FPlatformManageOptionalParam& InOptionParam)
{
	checkf(SessionResult.IsValid(),TEXT("Make sure session result is valid !"))
	
	if(SessionResult.IsValid())
	{
		if(UOnlineSessionsManage* InSubSystem = GetGLCOnlineSessionsSubsystem())
		{
			return InSubSystem->JoinSession(SessionResult, InOptionParam);
		}
	}
	else
	{
		UE_LOG(LogGLCOnlineManagementSystem,Error,TEXT("Cannot pass a Result that is not valid !"));
	}
	return EJoinSessionRequestState::JOIN_SESSION_REQUEST_FAIL;
}

EJoinSessionRequestState UGLCOnlineSessionsLibrary::K2_JoinSession(const FGLCOnlineSessionSearchResult& SessionResult
	, const FPlatformManageOptionalParam& InOptionParam)
{
	checkf(SessionResult.SessionInfo.IsValid(), TEXT("Make sure session result is valid !"))
	//Check password
	if(SessionResult.bPassword)
	{
		if(FMapSettings* InSettings = GetMapSettings())
		{
			if(UUI_Password* UI_Password = CreateWidget<UUI_Password>(UGLCPlatformManageLibrary::GetPlayerController(),
				InSettings->UI_Password))
			{
				UI_Password->Init(SessionResult,InOptionParam);
				UI_Password->AddToViewport(100);
				return EJoinSessionRequestState::Waiting_Password;
			}
			else
			{
				checkf(false,TEXT("Please ensure that the UI for entering the password has been set in the map settings"))
			}
		}
		return EJoinSessionRequestState::JOIN_SESSION_REQUEST_FAIL;
	}
	//
	if (SessionResult.SessionInfo.IsValid())
	{
		if (UOnlineSessionsManage* InSubSystem = GetGLCOnlineSessionsSubsystem())
		{
			if (const FOnlineSessionSearchResult* InResult = ConvertSearchResult(SessionResult))
			{
				return InSubSystem->JoinSession(*InResult, InOptionParam);
			}
		}
	}
	return EJoinSessionRequestState::JOIN_SESSION_REQUEST_FAIL;
}

bool UGLCOnlineSessionsLibrary::DestroySession(const FPlatformManageOptionalParam& InOptionParam)
{
	if (UOnlineSessionsManage* InSubSystem = GetGLCOnlineSessionsSubsystem())
	{
		return InSubSystem->DestroySession(InOptionParam);
	}
	return false;
}

bool UGLCOnlineSessionsLibrary::ClientJoinMap(APlayerController* InPlayerController)
{
	if(InPlayerController)
	{
		if(IOnlineSessionPtr InPtr = IOnlineSubsystem::Get()->GetSessionInterface())
		{
			FString ConnectString;
			if(InPtr->GetResolvedConnectString(NAME_GameSession,ConnectString))
			{
				//设置已经在会话里
				if (UGLCGameInstance* Gi = UGLCPlatformManageLibrary::GetGLCGameInstance())
				{
					Gi->SetIsInSession(true);
				}
				InPlayerController->ClientTravel(*ConnectString,TRAVEL_Absolute);
				return true;
			}
		}
	}
	return false;
}

bool UGLCOnlineSessionsLibrary::ServerJoinLobbyMap(APlayerController* InPlayerController,
	int32 LobbyIndex ,bool bRandomLobbyMap)
{
	if (InPlayerController && InPlayerController->GetWorld())
	{
		FString InLobbyPath;
		if(bRandomLobbyMap)
		{
			InLobbyPath = GetPathToLobby(true,INDEX_NONE);
		}
		else
		{
			InLobbyPath = GetPathToLobby(true,LobbyIndex);
		}
		
		if (!InLobbyPath.IsEmpty())
		{
			//设置已经在会话里
			if (UGLCGameInstance* Gi = UGLCPlatformManageLibrary::GetGLCGameInstance())
			{
				Gi->SetIsInSession(true);
			}
			return InPlayerController->GetWorld()->ServerTravel(*InLobbyPath);
		}
	}
	return false;
}

bool UGLCOnlineSessionsLibrary::ServerJoinGameMap(APlayerController* InPlayerController)
{
	if (InPlayerController && InPlayerController->GetWorld())
	{
		if (UGLCOnlineSessionsConfig* InConfig = GetOnlineSessionsConfigFromSlot())
		{
			FString OutPath;
			if (GetPathToGameMap(OutPath))
			{
				return InPlayerController->GetWorld()->ServerTravel(*OutPath);
			}
		}
	}
	return false;
}

bool UGLCOnlineSessionsLibrary::StartGame(APlayerController* InPlayerController,bool bCheckHasAuthority)
{
	auto MyStart = [&]()->bool
	{
		if (UGLCOnlineManageComponent* InSessionComponent = UGLCPlatformManageLibrary::GetGLCOnlineManageComponentFormObject(InPlayerController))
		{
			InSessionComponent->Server_StartGame();
			return true;
		}
		checkf(false,TEXT("[%s] does not have a GLCOnlineManagementComponent !") ,*InPlayerController->GetName())
		return false;
	};

	if (UWorld* InWorld = GetWorldFromObject(InPlayerController))
	{
		if (bCheckHasAuthority)
		{
			if (InWorld->GetNetMode() < ENetMode::NM_Client)
			{
				return MyStart();
			}
		}
		else
		{
			return MyStart();
		}
	}
	return false;
}

bool UGLCOnlineSessionsLibrary::StartGameByKey(UObject* InObject, int32 InKey, 
	bool bCheckHasAuthority /*= true*/)
{
	auto MyStart = [&]()->bool
	{
		if (UGLCOnlineManageComponent* InSessionComponent = UGLCPlatformManageLibrary::GetGLCOnlineManageComponentFormObject(InObject))
		{
			InSessionComponent->Server_StartGameByKey(InKey);
			return true;
		}
		return false;
	};

	if (UWorld* InWorld = GetWorldFromObject(InObject))
	{
		if (bCheckHasAuthority)
		{
			if (InWorld->GetNetMode() < ENetMode::NM_Client)
			{
				return MyStart();
			}
		}
		else
		{
			return MyStart();
		}
	}
	return false;
}

bool UGLCOnlineSessionsLibrary::GetPathToGameMap(FString& OutPath,bool bAddListen)
{
	if (UGLCOnlineSessionsConfig* InConfig = GetOnlineSessionsConfigFromSlot())
	{
		check(InConfig->OnlineSessionsConfigSpec.SessionCreateSettings.MapID != INDEX_NONE);
		if (InConfig->OnlineSessionsConfigSpec.SessionCreateSettings.MapID != INDEX_NONE)
		{
			if (FMapSettingSpec* InSpec = GetSettingSpecByKey(InConfig->OnlineSessionsConfigSpec.SessionCreateSettings.MapID))
			{
				OutPath = *InSpec->GetMapPath();
				if (bAddListen)
				{
					OutPath += TEXT("?listen");
				}
				return true;
			}
		}
	}
	return false;
}

UUserWidget* UGLCOnlineSessionsLibrary::CreateLobbyWidget(TSubclassOf<UUserWidget> InWidget,
	APlayerController* InPlayerController, bool bInputUIOnly
	,bool bShowMouse,bool bCheckHasAuthority,int32 ZOrder)
{
	auto CreateLobby = [&]()->UUserWidget*
	{
		if (InPlayerController)
		{
			if (bInputUIOnly)
			{
				UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(InPlayerController);
			}
			if (bShowMouse)
			{
				InPlayerController->SetShowMouseCursor(true);
			}
			if(InWidget)
			{
				if (UUserWidget* NewWidget = UWidgetBlueprintLibrary::Create(InPlayerController->GetWorld(),
				InWidget, InPlayerController))
				{
					NewWidget->AddToViewport(ZOrder);
					return NewWidget;
				}
			}
			else
			{
				if (UUserWidget* NewWidget = UWidgetBlueprintLibrary::Create(InPlayerController->GetWorld(),
					LobbyStartClass, InPlayerController))
				{
					NewWidget->AddToViewport(ZOrder);
					return NewWidget;
				}
			}
		}
		return nullptr;
	};
	if (bCheckHasAuthority)
	{
		if (InPlayerController && InPlayerController->HasAuthority())
		{
			return CreateLobby();
		}
	}
	else
	{
		return CreateLobby();
	}
	return nullptr;
}

bool UGLCOnlineSessionsLibrary::TryLogin(FString Account, FString Password, ELoginMethod LoginMethod, FPlatformManageOptionalParam InParam)
{
	return UOnlineSessionsManage::Get()->TryLogin(Account, Password, LoginMethod, InParam);
}

bool UGLCOnlineSessionsLibrary::Logout()
{
	return UOnlineSessionsManage::Get()->Logout();
}

bool UGLCOnlineSessionsLibrary::IsLogin()
{
	return UOnlineSessionsManage::Get()->IsLogin();
}

bool UGLCOnlineSessionsLibrary::IsInSession()
{
	if (UGLCGameInstance* Gi = UGLCPlatformManageLibrary::GetGLCGameInstance())
	{
		return Gi->IsInSession();
	}
	return false;
}

EjoinSessionCompleteType UGLCOnlineSessionsLibrary::GetJoinSessionCompleteType(EOnJoinSessionCompleteResult::Type InType)
{
	switch (InType)
	{
	case EOnJoinSessionCompleteResult::Success:
		return EjoinSessionCompleteType::Success;
		break;
	case EOnJoinSessionCompleteResult::SessionIsFull:
		return EjoinSessionCompleteType::SessionIsFull;
		break;
	case EOnJoinSessionCompleteResult::SessionDoesNotExist:
		return EjoinSessionCompleteType::SessionDoesNotExist;
		break;
	case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
		return EjoinSessionCompleteType::CouldNotRetrieveAddress;
		break;
	case EOnJoinSessionCompleteResult::AlreadyInSession:
		return EjoinSessionCompleteType::AlreadyInSession;
		break;
	case EOnJoinSessionCompleteResult::UnknownError:
		return EjoinSessionCompleteType::UnknownError;
		break;
	default:
		break;
	}
	return EjoinSessionCompleteType::UnknownError;
}

void UGLCOnlineSessionsLibrary::ConvertSearchResult(const TArray<FOnlineSessionSearchResult>& InResult, 
	TArray<FGLCOnlineSessionSearchResult>& OutResults)
{
	for (auto& Tem : InResult)
	{
		FGLCOnlineSessionSearchResult NewResult;

		NewResult.MaxPlayer = Tem.Session.SessionSettings.NumPublicConnections;
		NewResult.CurrentPlayer = NewResult.MaxPlayer - (Tem.Session.NumOpenPublicConnections);
		NewResult.NumPrivateOpenConnections = Tem.Session.NumOpenPrivateConnections;
		NewResult.NumPrivateConnections = Tem.Session.SessionSettings.NumPrivateConnections;
		NewResult.OwningUserName = Tem.Session.OwningUserName;
		NewResult.Ping = Tem.PingInMs;
		NewResult.SessionInfo = Tem.Session.SessionInfo;

		for (auto& Tem_ : Tem.Session.SessionSettings.Settings)
		{
			NewResult.Settings.SettingSpecs.Add(Tem_.Key,Tem_.Value.Data.ToString());
		}
		if (const FOnlineSessionSetting* InSetting = Tem.Session.SessionSettings.Settings.Find(*OnlineSessionStaticDef::RoomNameKey))
		{
			NewResult.RoomName = InSetting->Data.ToString();
		}
		if (const FOnlineSessionSetting* InSetting = Tem.Session.SessionSettings.Settings.Find(*OnlineSessionStaticDef::MapConfigKey))
		{
			InSetting->Data.GetValue(NewResult.MapID);
		}
		if (const FOnlineSessionSetting* InSetting = Tem.Session.SessionSettings.Settings.Find(*OnlineSessionStaticDef::RoomPassword))
		{
			NewResult.RoomPassword = InSetting->Data.ToString();
		}
		if (const FOnlineSessionSetting* InSetting = Tem.Session.SessionSettings.Settings.Find(*OnlineSessionStaticDef::bPassword))
		{
			InSetting->Data.GetValue(NewResult.bPassword);
		}

		OutResults.Add(NewResult);
	}
}

void UGLCOnlineSessionsLibrary::ConvertSearchResult(const TArray<FGLCOnlineSessionSearchResult>& InResult,
	TArray<FOnlineSessionSearchResult>& OutResults)
{
	if (UOnlineSessionsManage* InSubSystem = UGLCOnlineSessionsLibrary::GetGLCOnlineSessionsSubsystem())
	{
		for (auto& MainTem : InSubSystem->GetSearchResults())
		{
			for (auto& Tem : InResult)
			{
				if (Tem.SessionInfo == MainTem.Session.SessionInfo)
				{
					OutResults.Add(MainTem);
					break;
				}
			}
		}
	}
}

const FOnlineSessionSearchResult* UGLCOnlineSessionsLibrary::ConvertSearchResult(const FGLCOnlineSessionSearchResult& InResult)
{
	if (UOnlineSessionsManage* InSubSystem = UGLCOnlineSessionsLibrary::GetGLCOnlineSessionsSubsystem())
	{
		for (auto& Tem : InSubSystem->GetSearchResults())
		{
			if (Tem.Session.SessionInfo == InResult.SessionInfo)
			{
				return &Tem;
			}
		}
	}
	return nullptr;
}

void UGLCOnlineSessionsLibrary::ConvertCreateSessionSetting(const FGLCSessionCreateSettings& InCreateSetting,
	FOnlineSessionSettings& OutSettings)
{
	OutSettings.NumPublicConnections = InCreateSetting.NumPublicConnections;
	OutSettings.NumPrivateConnections = InCreateSetting.NumPrivateConnections;
	OutSettings.bShouldAdvertise = InCreateSetting.bShouldAdvertise;
	OutSettings.bAllowJoinInProgress = InCreateSetting.bAllowJoinInProgress;
	OutSettings.bIsDedicated = InCreateSetting.bIsDedicated;
	OutSettings.bUsesStats = InCreateSetting.bUsesStats;
	OutSettings.bAllowInvites = InCreateSetting.bAllowInvites;
	OutSettings.bUsesPresence = InCreateSetting.bUsesPresence;
	OutSettings.bAllowJoinViaPresence = InCreateSetting.bAllowJoinViaPresence;
	OutSettings.bAllowJoinViaPresenceFriendsOnly = InCreateSetting.bAllowJoinViaPresenceFriendsOnly;
	OutSettings.bAntiCheatProtected = InCreateSetting.bAntiCheatProtected;
	OutSettings.bUseLobbiesIfAvailable = InCreateSetting.bUseLobbiesIfAvailable;
	OutSettings.bUseLobbiesVoiceChatIfAvailable = InCreateSetting.bUseLobbiesVoiceChatIfAvailable;
	OutSettings.BuildUniqueId = InCreateSetting.BuildUniqueId;

	for (auto& Temp : InCreateSetting.OptionalParam.SettingSpecs)
	{
		OutSettings.Set(Temp.Key,Temp.Value,EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	}
	OutSettings.Set(*OnlineSessionStaticDef::MapConfigKey,InCreateSetting.MapID, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	OutSettings.Set(*OnlineSessionStaticDef::RoomNameKey, InCreateSetting.RoomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	OutSettings.Set(*OnlineSessionStaticDef::bPassword, InCreateSetting.bPassword, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	OutSettings.Set(*OnlineSessionStaticDef::RoomPassword, InCreateSetting.RoomPassword, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

}

UGLCOnlineManageComponent* UGLCOnlineSessionsLibrary::GetGLCOnlineManagerComponentFormObject(UObject* InObject)
{
	return UGLCPlatformManageLibrary::GetGLCOnlineManageComponentFormObject(InObject);
}

UGLCOnlineManageComponent* UGLCOnlineSessionsLibrary::GetGLCOnlineManagerComponent()
{
	return UGLCPlatformManageLibrary::GetGLCOnlineManagerComponent();
}
