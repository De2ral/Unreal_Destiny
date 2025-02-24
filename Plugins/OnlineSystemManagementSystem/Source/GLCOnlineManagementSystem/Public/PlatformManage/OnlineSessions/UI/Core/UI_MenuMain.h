#pragma once

#include "CoreMinimal.h"
#include "PlatformManage/OnlineSessions/UI/Core/UI_MenuBase.h"
#include "PlatformManage/OnlineSessions/Common/GLCOnlineSessionsType.h"

#include "UI_MenuMain.generated.h"



class UUI_SingleRusult;
class UUI_Loading;
class UUI_Startup;
class UUI_SearchList;
class UUI_ConfigSet;
struct FGLCOnlineSessionSearchResult;
class UUI_SingleRusult;
class UScrollBox;
class UButton;


/**
 *
 */
UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UUI_SearchList : public UUI_MenuBase
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* SearchScrollBox;

	UPROPERTY(EditAnywhere, Category = "SearchList", meta = (AllowPrivateAccess = "true"))
	FMargin PaddingBetweenResults;

public:
	UUI_SearchList();
protected:
	virtual void NativeConstruct()override;
	UFUNCTION()
	void CancelButtonPress();
public:
	void ClearAllResult();
	void AddNewResult(UUI_SingleRusult* InNewResult);
	bool IsResultEmpty();

};


UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UUI_MenuMain : public UUI_MenuBase
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	UUI_Startup* UI_Startup;
	UPROPERTY(meta = (BindWidget))
	UUI_SearchList* UI_SearchList;
	UPROPERTY(meta = (BindWidget))
	UUI_ConfigSet* UI_ConfigSet;
	UPROPERTY(meta = (BindWidget))
	UUI_Loading* UI_Loading;

	UPROPERTY(EditAnywhere,Category = "UI_MenuMain")
	TSubclassOf<UUI_SingleRusult> ResultClass;

public:
	UPROPERTY(EditAnywhere,Category = "UI_MenuMain")
	bool bDebug;

public:
	UUI_MenuMain();
	void PlayConfigInAnim();
	void PlayConfigOutAnim();
	void PlaySearchInAnim();
	void PlaySearchOutAnim();

	void CreateSession(const FGLCSessionCreateSettings& InSettings);
	UFUNCTION(BlueprintNativeEvent)
	void OnCreateSessionComplete(bool bWasSuccessful,const FPlatformManageOptionalParam& InParam);

	UFUNCTION()
	void FindSession();
	UFUNCTION(BlueprintNativeEvent)
	void OnFindSessionComplete(const TArray<FGLCOnlineSessionSearchResult>& SessionResults,
		bool bWasSuccessful, const FPlatformManageOptionalParam& InParam);

	void JoinSession(const FGLCOnlineSessionSearchResult& SessionResult);
	UFUNCTION(BlueprintNativeEvent)
	void OnJoinSessionComplete(EjoinSessionCompleteType Type, const FPlatformManageOptionalParam& InParam);

	void StopLoadingAnim();
	void ShowLoading();
	UFUNCTION(BlueprintCallable,Category = "UI_MenuMain")
	void HideLoading();
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct()override;

};
