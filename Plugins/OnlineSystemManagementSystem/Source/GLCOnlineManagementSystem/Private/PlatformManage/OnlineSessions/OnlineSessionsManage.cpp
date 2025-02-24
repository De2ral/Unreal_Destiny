#include "PlatformManage/OnlineSessions/OnlineSessionsManage.h"
#include "OnlineSubsystem.h"
#include "Engine/GameInstance.h"
#include "Common/Components/GLCOnlineManageComponent.h"
#include <PlatformManage/OnlineSessions/Common/GLCOnlineSessionsLibrary.h>
#include <PlatformManage/OnlineSessions/UI/Core/UI_MenuMain.h>
#include <PlatformManage/Common/GLCPlatformManageLibrary.h>
#include "GameFramework/OnlineReplStructs.h"
#include "Common/GLCGameInstance.h"
#include "PlatformManage/OnlineSessions/Common/GLCOnlineSessionsType.h"


static EGLCLoginState LoginState = EGLCLoginState::NotLoggedIn;


UOnlineSessionsManage::UOnlineSessionsManage(const FObjectInitializer& Initializer):
	Super(Initializer)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		ClearAllHandle();
		IdentityPtr = Subsystem->GetIdentityInterface();
		SessionInterface = Subsystem->GetSessionInterface();
		
		if (SessionInterface.IsValid() && IdentityPtr.IsValid())
		{
			IdentityPtr->AddOnLoginCompleteDelegate_Handle(0, FOnLoginCompleteDelegate::CreateWeakLambda(this,
				[&](int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
				{
					RemoveRequest(OnlineSessionStaticDef::OnLoginCompleteKey,
					[&](const FPlatformManageOptionalParam& Param)
						{
							FUniqueNetIdRepl NetID;
							if (UserId.IsValid())
							{
								NetID = FUniqueNetIdRepl(UserId);
							}
							else
							{
								UGLCPlatformManageLibrary::GetUniqueNetID(NetID);
							}
							for (auto& Temp : GetOnlineManageComponent()->GLCOnlineSessionObjectClasses)
							{
								Temp.GetDefaultObject()->OnLoginComplete(LocalUserNum,
									bWasSuccessful, NetID, Error, Param);
							}
						});
					
				}));

			IdentityPtr->AddOnLoginStatusChangedDelegate_Handle(0, FOnLoginStatusChangedDelegate::CreateWeakLambda(this,
				[&](int32 LocalUserNum, ELoginStatus::Type OldStatus, ELoginStatus::Type NewStatus, const FUniqueNetId& NewId)
				{
					if (LoginState != (EGLCLoginState)NewStatus)
					{
						LoginState = (EGLCLoginState)NewStatus;
						
						FUniqueNetIdRepl RID = FUniqueNetIdRepl(NewId);
						for (auto& Temp : GetOnlineManageComponent()->GLCOnlineSessionObjectClasses)
						{
							Temp.GetDefaultObject()->OnLogStatusChanged(LocalUserNum, (EGLCLoginState)OldStatus, (EGLCLoginState)NewStatus, RID);
						}
					}
				}));
			
			SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateWeakLambda(this,
			[&](FName SessionName, bool bWasSuccessful)
			{
				if (GetOnlineManageComponent() && GetOnlineManageComponent()->GLCOnlineSessionObjectClasses.Num())
				{
					RemoveRequest(OnlineSessionStaticDef::OnCreateSessionCompleteKey,
					[&](const FPlatformManageOptionalParam& Param)
					{
						for (auto& Temp : GetOnlineManageComponent()->GLCOnlineSessionObjectClasses)
						{
							Temp.GetDefaultObject()->OnCreateSessionComplete
							(bWasSuccessful, Param);
						}
						if (UUI_MenuMain* InMenuMain = UGLCOnlineSessionsLibrary::GetMenu())
						{
							InMenuMain->OnCreateSessionComplete(bWasSuccessful, Param);
						}
					});
				}
			}));

			SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateWeakLambda(this,
			[&](bool bWasSuccessful)
			{
				if (GetOnlineManageComponent() && GetOnlineManageComponent()->GLCOnlineSessionObjectClasses.Num())
				{
					RemoveRequest(OnlineSessionStaticDef::OnFindSessionCompleteKey,
					[&](const FPlatformManageOptionalParam& Param)
					{
						TArray<FGLCOnlineSessionSearchResult> BPSearchResults;
						UGLCOnlineSessionsLibrary::ConvertSearchResult(GetSearchResults(), BPSearchResults);

						for (auto& Temp : GetOnlineManageComponent()->GLCOnlineSessionObjectClasses)
						{
							Temp.GetDefaultObject()->OnFindSessionComplete
							(BPSearchResults, bWasSuccessful, Param);
						}
						if (UUI_MenuMain* InMenuMain = UGLCOnlineSessionsLibrary::GetMenu())
						{
							InMenuMain->OnFindSessionComplete(BPSearchResults, bWasSuccessful, Param);
						}
					});
				}
			}));

			SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateWeakLambda(this,
			[&](FName SessionName, EOnJoinSessionCompleteResult::Type Result)
			{
				if (GetOnlineManageComponent() && GetOnlineManageComponent()->GLCOnlineSessionObjectClasses.Num())
				{
					RemoveRequest(OnlineSessionStaticDef::OnJoinSessionCompleteKey,
					[&](const FPlatformManageOptionalParam& Param)
					{
						for (auto& Temp : GetOnlineManageComponent()->GLCOnlineSessionObjectClasses)
						{
							Temp.GetDefaultObject()->OnJoinSessionComplete
							(UGLCOnlineSessionsLibrary::GetJoinSessionCompleteType(Result), Param);
						}
						if (UUI_MenuMain* InMenuMain = UGLCOnlineSessionsLibrary::GetMenu())
						{
							InMenuMain->OnJoinSessionComplete(UGLCOnlineSessionsLibrary::GetJoinSessionCompleteType(Result)
								, Param);
						}
					});
				}
			}));

			SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(FOnDestroySessionCompleteDelegate::CreateWeakLambda(this,
			[&](FName SessionName, bool bWasSuccessful)
			{
				//设置会话结束
				if (UGLCGameInstance* Gi = UGLCPlatformManageLibrary::GetGLCGameInstance())
				{
					Gi->SetIsInSession(false);
				}
				
				RemoveRequest(OnlineSessionStaticDef::OnDestroySessionCompleteKey, 
				[&](const FPlatformManageOptionalParam& Param) 
				{
					if (bWasSuccessful && bCreateSessionOnDestroy)
					{
						bCreateSessionOnDestroy = false;
						CreateSession_(LastCreateSettings, LastCreateSessionOptionalParam);
					}
					else if (bWasSuccessful && bJoinSessionOnDestroy)
					{
						bJoinSessionOnDestroy = false;
						JoinSession(LastSearchResult, LastJoinSessionOptionalParam);
					}
					else
					{
						for (auto& Temp : GetOnlineManageComponent()->GLCOnlineSessionObjectClasses)
						{
							Temp.GetDefaultObject()->OnDestroySessionComplete
							(bWasSuccessful,Param);
						}
					}
				});

			}));
		}
	}

	bCreateSessionOnDestroy = false;
	bJoinSessionOnDestroy	= false;
}

UOnlineSessionsManage::~UOnlineSessionsManage()
{
	ClearAllHandle();
	SessionInterface.Reset();
	IdentityPtr.Reset();
}

void UOnlineSessionsManage::ClearAllHandle()
{
	if (SessionInterface.IsValid() && IdentityPtr.IsValid())
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegates(this);
		SessionInterface->ClearOnJoinSessionCompleteDelegates(this);
		SessionInterface->ClearOnFindSessionsCompleteDelegates(this);
		SessionInterface->ClearOnDestroySessionCompleteDelegates(this);

		IdentityPtr->ClearOnLoginCompleteDelegates(0,this);
		IdentityPtr->ClearOnLoginStatusChangedDelegates(0, this);

		IdentityPtr.Reset();
		SessionInterface.Reset();
	}
}

ECreateSessionRequestState UOnlineSessionsManage::CreateSession_(const FGLCSessionCreateSettings& InSetting,
	const FPlatformManageOptionalParam& InOptionalParam)
{
	if (!SessionInterface.IsValid())
	{
		return ECreateSessionRequestState::CREATE_SESSTION_REQUEST_FAIL;
	}
	/*if (!IsLogin())
	{
		TryLogin(FString(), FString());
		return ECreateSessionRequestState::CREATE_SESSTION_REQUEST_FAIL;
	}*/
	FName SubSystemName = IOnlineSubsystem::Get()->GetSubsystemName();
	if (SubSystemName == TEXT("EOS") || SubSystemName == TEXT("EOSPlus"))
	{
		int32 Num = InSetting.NumPublicConnections + InSetting.NumPrivateConnections;
		if (Num > 16)
		{
			checkf(false, TEXT("The max number of players must be less than or equal to 16"));
			return ECreateSessionRequestState::CREATE_SESSTION_REQUEST_FAIL;
		}
	}

	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		bCreateSessionOnDestroy = true;
		LastCreateSettings = InSetting;
		LastCreateSessionOptionalParam = InOptionalParam;

		if (!DestroySession())
		{
			bCreateSessionOnDestroy = false;
			return ECreateSessionRequestState::CREATE_SESSTION_REQUEST_FAIL;
		}
		return ECreateSessionRequestState::ALREADY_IN_SESSION_DESTROYING_SESSION_AND_RECREATING;
	}

	if (!PublishRequest(LastCreateSessionOptionalParam, OnlineSessionStaticDef::OnCreateSessionCompleteKey))
	{
		return ECreateSessionRequestState::CREATE_SESSTION_REQUEST_FAIL;
	}

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = SubSystemName == "NULL" ? true : false;
	UGLCOnlineSessionsLibrary::ConvertCreateSessionSetting(InSetting,*LastSessionSettings.Get());
	
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	FUniqueNetIdRepl ID = LocalPlayer->GetPreferredUniqueNetId();
	checkf(ID.IsValid(), TEXT("The id is not valid.Please login first"));

	if(ID.IsValid())
	{
		if (SessionInterface->CreateSession(*ID,
		NAME_GameSession, *LastSessionSettings))
		{
			return ECreateSessionRequestState::CREATE_SESSTION_REQUEST_SUCCESSFUL;
		}
	}

	UE_LOG(LogGLCOnlineManagementSystem,Error,TEXT("The id is not valid.Please login first"))
	RemoveRequest(OnlineSessionStaticDef::OnCreateSessionCompleteKey, [&](const FPlatformManageOptionalParam& Param) {});
	return ECreateSessionRequestState::CREATE_SESSTION_REQUEST_FAIL;
}

bool UOnlineSessionsManage::FindSessions(int32 MaxSearchResults, 
	const FPlatformManageOptionalParam& InOptionalParam)
{
	if (!SessionInterface.IsValid())
	{
		return false;
	}
	/*if (!IsLogin())
	{
		TryLogin(FString(), FString());
		return false;
	}*/

	if (!PublishRequest(InOptionalParam, OnlineSessionStaticDef::OnFindSessionCompleteKey))
	{
		return false;
	}
	LastSessionSearchResults = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearchResults->MaxSearchResults = MaxSearchResults;
	LastSessionSearchResults->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	/*LastSessionSearchResults->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	LastSessionSearchResults->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);*/
	LastSessionSearchResults->QuerySettings.Set(FName(TEXT("PRESENCESEARCH")), true, EOnlineComparisonOp::Equals);
	LastSessionSearchResults->QuerySettings.Set(FName(TEXT("LOBBYSEARCH")), true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(),
		LastSessionSearchResults.ToSharedRef()))
	{
		return true;
	}
	RemoveRequest(OnlineSessionStaticDef::OnFindSessionCompleteKey, [&](const FPlatformManageOptionalParam& Param) {});
	return false;
}

EJoinSessionRequestState UOnlineSessionsManage::JoinSession(const FOnlineSessionSearchResult& SessionResult,
	const FPlatformManageOptionalParam& InOptionalParam)
{
	/*if (!IsLogin())
	{
		TryLogin(FString(), FString());
		return EJoinSessionRequestState::JOIN_SESSION_REQUEST_FAIL;
	}*/

	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if(ExistingSession != nullptr)
	{
		LastSearchResult = SessionResult;
		LastJoinSessionOptionalParam = InOptionalParam;
		bJoinSessionOnDestroy = true;
		if(!DestroySession())
		{
			bJoinSessionOnDestroy = false;
			return EJoinSessionRequestState::JOIN_SESSION_REQUEST_FAIL;
		}
		else
		{
			return EJoinSessionRequestState::ALREADY_IN_SESSION_DESTROYING_SESSION_AND_REJOIN;
		}
	}

	if (!PublishRequest(InOptionalParam, OnlineSessionStaticDef::OnJoinSessionCompleteKey))
	{
		return EJoinSessionRequestState::JOIN_SESSION_REQUEST_FAIL;
	}

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		return EJoinSessionRequestState::JOIN_SESSION_REQUEST_SUCCESSFUL;
	}
	FString InKey = OnlineSessionStaticDef::OnJoinSessionCompleteKey;
	RemoveRequest(InKey,[&](const FPlatformManageOptionalParam& Param){});
	return EJoinSessionRequestState::JOIN_SESSION_REQUEST_FAIL;
}

bool UOnlineSessionsManage::DestroySession(const FPlatformManageOptionalParam InParam)
{
	if (!SessionInterface.IsValid())
	{
		return false;
	}
	/*if (!IsLogin())
	{
		TryLogin(FString(), FString());
		return false;
	}*/
	
	if (!PublishRequest(InParam, OnlineSessionStaticDef::OnDestroySessionCompleteKey))
	{
		return false;
	}
	if (SessionInterface->DestroySession(NAME_GameSession))
	{
		return true;
	}
	RemoveRequest(OnlineSessionStaticDef::OnDestroySessionCompleteKey,[&](const FPlatformManageOptionalParam& Param) {});
	return false;
}

void UOnlineSessionsManage::StartSession()
{
}

UOnlineSessionsManage* UOnlineSessionsManage::UOnlineSessionsManage::Get()
{
	if(UGameInstance* InGI = UGLCPlatformManageLibrary::GetGameInstance())
	{
		return InGI->GetSubsystem<UOnlineSessionsManage>();
	}
	return nullptr;
}

bool UOnlineSessionsManage::IsLogin()
{
	if (IdentityPtr.IsValid())
	{
		return IdentityPtr->GetLoginStatus(0) == ELoginStatus::LoggedIn;
	}
	return false;
}

bool UOnlineSessionsManage::TryLogin(FString Account, FString Password, ELoginMethod LoginMethod, const FPlatformManageOptionalParam& InParam)
{
	if (IdentityPtr.IsValid())
	{
		if (!PublishRequest(InParam, OnlineSessionStaticDef::OnLoginCompleteKey))
		{
			RemoveRequest(OnlineSessionStaticDef::OnLoginCompleteKey,
				[&](const FPlatformManageOptionalParam& Param) {});
			PublishRequest(InParam, OnlineSessionStaticDef::OnLoginCompleteKey);
		}

		FOnlineAccountCredentials Creadentials;
		Creadentials.Id = Account;
		Creadentials.Token = Password;
		//Switch str
		switch (LoginMethod)
		{
		case ELoginMethod::ExchangeCode:
			Creadentials.Type = TEXT("exchangecode");
			break;
		case ELoginMethod::AccountAndPassword:
			Creadentials.Type = TEXT("password");
			break;
		case ELoginMethod::AccountPortal:
			Creadentials.Type = TEXT("accountportal");
			break;
		case ELoginMethod::PersistentAuth:
			Creadentials.Type = TEXT("persistentauth");
			break;
		default:
			break;
		}

		if (IOnlineSubsystem::Get()->GetSubsystemName() == TEXT("EOSPlus"))
		{
			Creadentials.Type = TEXT("");
		}
		return IdentityPtr->Login(0, Creadentials);
	}
	return false;
}

bool UOnlineSessionsManage::Logout()
{
	if (IdentityPtr.IsValid())
	{
		IdentityPtr->Logout(0);
	}


	return false;
}

