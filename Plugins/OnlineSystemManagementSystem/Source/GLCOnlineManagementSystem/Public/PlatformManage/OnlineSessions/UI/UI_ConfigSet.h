#pragma once

#include "CoreMinimal.h"
#include "PlatformManage/OnlineSessions/UI/Core/UI_MenuBase.h"
#include "UI_ConfigSet.generated.h"

class UHorizontalBox;
class UVerticalBox;
class UButton;
class UEditableTextBox;
class UComboBoxString;
class USpinBox;
class UMultiplayerComboBoxString;

/**
 * 
 */
UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UUI_ConfigSet : public UUI_MenuBase
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* RootNameText;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* GameModeComboBox;
	UPROPERTY(meta = (BindWidget))
	USpinBox* MaxPlayerNum;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* AllowJoinInProgressComboString;

	//Password
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* UI_PasswordCombo;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* RoomPassword;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* PasswordHorizontalBox_2;

protected:
	UUI_ConfigSet(); 
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CancelButtonPress();
	UFUNCTION()
	void OnRootPasswordTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnUI_PasswordComboChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void OnRootNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnGameModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void OnPlayerNumCommitted(float InValue, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnAllowJoinInProgressComboStringChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	void ModifyConfig();
public:
	UComboBoxString* GetGameModeComboBox() const { return GameModeComboBox; }
	UComboBoxString* GetAllowJoinInProgressComboString() const { return AllowJoinInProgressComboString; }

	bool bInit;
};
