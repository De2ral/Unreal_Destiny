#include "PlatformManage/OnlineSessions/UI/Core/UI_MenuMain.h"

#include "PlatformManage/OnlineSessions/OnlineSessionsManage.h"
#include "GLCRuntimeGamePrintingLibrary.h"
#include "PlatformManage/OnlineSessions/UI/UI_ConfigSet.h"
#include "PlatformManage/OnlineSessions/UI/UI_Loading.h"
#include "PlatformManage/OnlineSessions/UI/UI_Startup.h"
#include "PlatformManage/OnlineSessions/UI/Element/UI_SingleRusult.h"
#include "PlatformManage/Common/GLCPlatformManageLibrary.h"
#include <PlatformManage/OnlineSessions/Common/GLCOnlineSessionsLibrary.h>
#include <Components/ScrollBoxSlot.h>
#include "Components/Button.h"
#include "Components/ScrollBox.h"


#define LOCTEXT_NAMESPACE "UUI_MenuMain"

UUI_SearchList::UUI_SearchList()
{
	PaddingBetweenResults = FMargin(8);
}

void UUI_SearchList::NativeConstruct()
{
	if (CancelButton)
	{
		CancelButton->OnReleased.AddDynamic(this, &UUI_SearchList::CancelButtonPress);
	}
}

void UUI_SearchList::CancelButtonPress()
{
	if (UUI_MenuMain* InMain = GetFather())
	{
		InMain->PlaySearchOutAnim();
	}
}

void UUI_SearchList::ClearAllResult()
{
	if (SearchScrollBox)
	{
		SearchScrollBox->ClearChildren();
	}
}

void UUI_SearchList::AddNewResult(UUI_SingleRusult* InNewResult)
{
	if (SearchScrollBox)
	{
		if (UScrollBoxSlot* NewSlot = Cast<UScrollBoxSlot>(SearchScrollBox->AddChild(InNewResult)))
		{
			NewSlot->SetPadding(PaddingBetweenResults);
		}
	}
}

bool UUI_SearchList::IsResultEmpty()
{
	if (SearchScrollBox)
	{
		return SearchScrollBox->GetChildrenCount() == 0;
	}
	return false;
}


UUI_MenuMain::UUI_MenuMain()
{
	bDebug = false;
}

void UUI_MenuMain::NativeConstruct()
{
	Super::NativeConstruct();

	if(UI_Startup && UI_SearchList && UI_ConfigSet && UI_Loading)
	{
		UI_Startup->SetFather(this);
		UI_SearchList->SetFather(this);
		UI_ConfigSet->SetFather(this);
		UI_Loading->SetFather(this);
	}
}

void UUI_MenuMain::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_MenuMain::PlayConfigInAnim()
{
	PlayWidgetAnimForward(TEXT("ConfigIn"));
}

void UUI_MenuMain::PlayConfigOutAnim()
{
	PlayWidgetAnimForward(TEXT("ConfigOut"));
}

void UUI_MenuMain::PlaySearchInAnim()
{
	PlayWidgetAnimForward(TEXT("SearchIn"));
}

void UUI_MenuMain::PlaySearchOutAnim()
{
	PlayWidgetAnimForward(TEXT("SearchOut"));
}

void UUI_MenuMain::CreateSession(const FGLCSessionCreateSettings& InSettings)
{
	if (GetWorld())
	{
		ECreateSessionRequestState Result = UGLCOnlineSessionsLibrary::CreateSession(InSettings,{});
		switch (Result)
		{
		case ECreateSessionRequestState::CREATE_SESSTION_REQUEST_FAIL:
			UGLCRuntimeGamePrintingLibrary::PrintRedLog(this,LOCTEXT("CREATE_SESSTION_REQUEST_FAIL","Create Session request fail !!!"));
			HideLoading();
			break;
		case ECreateSessionRequestState::ALREADY_IN_SESSION_DESTROYING_SESSION_AND_RECREATING:
			UGLCRuntimeGamePrintingLibrary::PrintWhiteLog(this,LOCTEXT("ALREADY_IN_SESSION_DESTROYING_SESSION_AND_RECREATING", "Already in session.Destroying the session and recreating"));
			break;
		case ECreateSessionRequestState::CREATE_SESSTION_REQUEST_SUCCESSFUL:
			UGLCRuntimeGamePrintingLibrary::PrintWhiteLog(this,LOCTEXT("CREATE_SESSTION_REQUEST_SUCCESSFUL", "Create session request successful !"));
			break;
		default:
			break;
		}
	}
}

void UUI_MenuMain::OnCreateSessionComplete_Implementation(bool bWasSuccessful,const FPlatformManageOptionalParam& InParam)
{
	if (bWasSuccessful && GetWorld())
	{
		if (!UGLCOnlineSessionsLibrary::ServerJoinLobbyMap(GetWorld()->GetFirstPlayerController()))
		{
			
		}
	}
	else
	{
		UGLCRuntimeGamePrintingLibrary::PrintRedLog(this,LOCTEXT("UI_MenuMain_CreateSession", "Failed to create session"));
	}
	HideLoading();
}

void UUI_MenuMain::FindSession()
{
	if (bDebug)
	{
		UI_SearchList->ClearAllResult();
		int32 Total = 30;
		for (int32 i = 0; i < Total; ++i)
		{
			if (UUI_SingleRusult* NewResult = CreateWidget<UUI_SingleRusult>(GetWorld(), ResultClass))
			{
				FGLCOnlineSessionSearchResult TestResult;
				TestResult.Ping = 60;
				TestResult.MaxPlayer = FMath::RandRange(20, 40);
				TestResult.CurrentPlayer = FMath::RandRange(3, 19);
				TestResult.MapID = FMath::RandRange(0, 2);
				TestResult.RoomName = FString::Printf(TEXT("Room_%d"), i);

				NewResult->SetFather(this);
				if (NewResult && NewResult->InitResult(TestResult))
				{
					UI_SearchList->AddNewResult(NewResult);
				}
			}

		}
		HideLoading();
		UGLCRuntimeGamePrintingLibrary::PrintWhiteLog(this,FText::Format(LOCTEXT("UI_MenuMain_FindSessionNum", "Total sessions found {0}"), Total));
	}
	else
	{
		if (!UGLCOnlineSessionsLibrary::FindSession({}))
		{
			HideLoading();
			UGLCRuntimeGamePrintingLibrary::PrintWhiteLog(this,LOCTEXT("FAIL_TO_FIND_SESSIONS", "Fail to find sessions"));
		}
	}
}

void UUI_MenuMain::OnFindSessionComplete_Implementation(const TArray<FGLCOnlineSessionSearchResult>& SessionResults, 
	bool bWasSuccessful, const FPlatformManageOptionalParam& InParam)
{
	if (bWasSuccessful)
	{
		if (UI_SearchList)
		{
			UI_SearchList->ClearAllResult();

			for (auto& Tem : SessionResults)
			{
				if (UUI_SingleRusult* NewResult = CreateWidget<UUI_SingleRusult>(GetWorld(), ResultClass))
				{
					NewResult->SetFather(this);
					if (NewResult->InitResult(Tem))
					{
						UI_SearchList->AddNewResult(NewResult);
					}
				}
			}
			UGLCRuntimeGamePrintingLibrary::PrintWhiteLog(this,FText::Format(LOCTEXT("UI_MenuMain_FindSessionNum", "Total sessions found {0}"), SessionResults.Num()));
		}
	}
	else
	{
		UGLCRuntimeGamePrintingLibrary::PrintRedLog(this,LOCTEXT("UI_MenuMain_FindSession", "Search session failed"));
	}
	HideLoading();
}

void UUI_MenuMain::JoinSession(const FGLCOnlineSessionSearchResult& SessionResult)
{
	checkf(SessionResult.SessionInfo.IsValid(),TEXT("Make sure the result is valid"));
	if(SessionResult.SessionInfo.IsValid() && GetWorld())
	{
		EJoinSessionRequestState InJoinState = UGLCOnlineSessionsLibrary::K2_JoinSession(SessionResult,{});
		switch (InJoinState)
		{
		case EJoinSessionRequestState::JOIN_SESSION_REQUEST_FAIL:
			UGLCRuntimeGamePrintingLibrary::PrintRedLog(this,LOCTEXT("UI_MenuMain_JoinSession_Fail", "Failed to join the session, unknown error"));
			HideLoading();
			break;
		case EJoinSessionRequestState::ALREADY_IN_SESSION_DESTROYING_SESSION_AND_REJOIN:
			UGLCRuntimeGamePrintingLibrary::PrintRedLog(this,LOCTEXT("ALREADY_IN_SESSION_DESTROYING_SESSION_AND_REJOIN", "Already in session.Destroying the session and rejoin"));
			break;
		case EJoinSessionRequestState::JOIN_SESSION_REQUEST_SUCCESSFUL:
			UGLCRuntimeGamePrintingLibrary::PrintWhiteLog(this,LOCTEXT("JOIN_SESSION_REQUEST_SUCCESSFUL", "Join session request successful"));
			break;
		default:
			break;
		}
	}
}

void UUI_MenuMain::OnJoinSessionComplete_Implementation(EjoinSessionCompleteType Type,
	const FPlatformManageOptionalParam& InParam)
{
	switch (Type)
	{
	case EjoinSessionCompleteType::Success:
		if (UWorld* InWorld = GetWorld())
		{
			UGLCOnlineSessionsLibrary::ClientJoinMap(InWorld->GetFirstPlayerController());
		}
		UGLCRuntimeGamePrintingLibrary::PrintWhiteLog(this,LOCTEXT("JoinSuccessful", "Joined successfully"));
		break;
	case EjoinSessionCompleteType::SessionIsFull:
		UGLCRuntimeGamePrintingLibrary::PrintYellowLog(this,LOCTEXT("SessionIsFull", "The room is full"));
		break;
	case EjoinSessionCompleteType::SessionDoesNotExist:
		UGLCRuntimeGamePrintingLibrary::PrintYellowLog(this,LOCTEXT("SessionDoesNotExist", "Room does not exist"));
		break;
	case EjoinSessionCompleteType::CouldNotRetrieveAddress:
		UGLCRuntimeGamePrintingLibrary::PrintYellowLog(this,LOCTEXT("CouldNotRetrieveAddress", "The address of the joined room is incorrect"));
		break;
	case EjoinSessionCompleteType::AlreadyInSession:
		UGLCRuntimeGamePrintingLibrary::PrintYellowLog(this,LOCTEXT("AlreadyInSession", "Already in the room"));
		break;
	case EjoinSessionCompleteType::UnknownError:
		UGLCRuntimeGamePrintingLibrary::PrintRedLog(this,LOCTEXT("UnknownError", "Join failed, unknown error"));
		break;
	default:
		break;
	}
	if (Type != EjoinSessionCompleteType::Success)
	{
		HideLoading();
	}
}

void UUI_MenuMain::StopLoadingAnim()
{
	if (UWidgetAnimation* MyTempAnim = GetNameWidgetAnimation(TEXT("LoadingIn")))
	{
		StopAnimation(MyTempAnim);
	}
	if (UWidgetAnimation* MyTempAnim = GetNameWidgetAnimation(TEXT("LoadingOut")))
	{
		StopAnimation(MyTempAnim);
	}
}

void UUI_MenuMain::ShowLoading()
{
	StopLoadingAnim();
	PlayWidgetAnimForward(TEXT("LoadingIn"));
}

void UUI_MenuMain::HideLoading()
{
	StopLoadingAnim();
	PlayWidgetAnimForward(TEXT("LoadingOut"));
}

#undef LOCTEXT_NAMESPACE
