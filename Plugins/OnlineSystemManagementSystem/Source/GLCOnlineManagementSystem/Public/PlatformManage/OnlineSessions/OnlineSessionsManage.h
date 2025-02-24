#pragma once
#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "PlatformManage/Common/OnlineSubsystemManage.h"
#include "PlatformManage/OnlineSessions/Common/GLCOnlineSessionsType.h"

#include "OnlineSessionsManage.generated.h"

class UGLCOnlineManageComponent;
class FOnlineSessionSearchResult;
struct FPlatformManageOptionalParam;
class FOnlineSessionSettings;
struct FGLCSessionCreateSettings;
enum class EjoinSessionCompleteType : uint8;
enum class EJoinSessionRequestState : uint8;


// Delcaring our own custom delegates for the Menu class to bind callbacks to

/**
 * 
 */
UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UOnlineSessionsManage : public UOnlineSubsystemManage
{
	GENERATED_BODY()
public:
	UOnlineSessionsManage(const FObjectInitializer& Initializer);
	~UOnlineSessionsManage();

	void ClearAllHandle();

	ECreateSessionRequestState CreateSession_(const FGLCSessionCreateSettings& InSetting,const FPlatformManageOptionalParam& InOptionalParam);
	bool FindSessions(int32 MaxSearchResults,const FPlatformManageOptionalParam& InOptionalParam);
	EJoinSessionRequestState JoinSession(const FOnlineSessionSearchResult& SessionResult,const FPlatformManageOptionalParam& InOptionalParam);

	bool DestroySession(const FPlatformManageOptionalParam InParam = FPlatformManageOptionalParam());
	void StartSession();


	static UOnlineSessionsManage* Get();

	//Get Search Result
	const TArray<FOnlineSessionSearchResult>& GetSearchResults() const { return LastSessionSearchResults->SearchResults; }
public:
	bool IsLogin();
	bool TryLogin(FString Account,FString Password, ELoginMethod LoginMethod,
		const FPlatformManageOptionalParam& InParam = FPlatformManageOptionalParam());
	bool Logout();

public:

private:
	IOnlineSessionPtr SessionInterface;
	IOnlineIdentityPtr IdentityPtr;
	
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearchResults;

	bool bCreateSessionOnDestroy;
	FGLCSessionCreateSettings LastCreateSettings;
	FPlatformManageOptionalParam LastCreateSessionOptionalParam;

	FOnlineSessionSearchResult LastSearchResult;
	FPlatformManageOptionalParam LastJoinSessionOptionalParam;
	bool bJoinSessionOnDestroy;

};
