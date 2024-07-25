

#include "PlatformManage/OnlineSessions/UI/UI_ConfigSet.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/SpinBox.h"
#include "PlatformManage/OnlineSessions/UI/Core/UI_MenuMain.h"
#include "Components/ComboBoxString.h"
#include <PlatformManage/OnlineSessions/Common/GLCOnlineSessionsLibrary.h>

#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "PlatformManage/OnlineSessions/SaveGame/GLCOnlineSessionsConfig.h"


UUI_ConfigSet::UUI_ConfigSet()
{
	bInit = false;
}

void UUI_ConfigSet::NativeConstruct()
{
	Super::NativeConstruct();

	if(CancelButton)
	{
		CancelButton->OnReleased.AddDynamic(this,&UUI_ConfigSet::CancelButtonPress);
	}
	if(RootNameText)
	{
		RootNameText->OnTextCommitted.AddDynamic(this,&UUI_ConfigSet::OnRootNameTextCommitted);
	}
	if(RoomPassword)
	{
		RoomPassword->OnTextCommitted.AddDynamic(this,&UUI_ConfigSet::OnRootPasswordTextCommitted);
	}
	if(GetGameModeComboBox())
	{
		GetGameModeComboBox()->OnSelectionChanged.AddDynamic(this,&UUI_ConfigSet::OnGameModeChanged);
	}
	if(MaxPlayerNum)
	{
		MaxPlayerNum->OnValueCommitted.AddDynamic(this,&UUI_ConfigSet::OnPlayerNumCommitted);
	}
	if(UI_PasswordCombo)
	{
		UI_PasswordCombo->OnSelectionChanged.AddDynamic(this,&UUI_ConfigSet::OnUI_PasswordComboChanged);
	}
	if(GetAllowJoinInProgressComboString())
	{
		GetAllowJoinInProgressComboString()->OnSelectionChanged.AddDynamic(this,&UUI_ConfigSet::OnAllowJoinInProgressComboStringChanged);
	}

	if (UGLCOnlineSessionsConfig* InConfig = UGLCOnlineSessionsLibrary::GetOnlineSessionsConfigFromSlot())
	{
		if (RootNameText && MaxPlayerNum && AllowJoinInProgressComboString && GameModeComboBox)
		{
			RootNameText->SetText(FText::FromString(InConfig->OnlineSessionsConfigSpec.SessionCreateSettings.RoomName));
			MaxPlayerNum->SetValue(InConfig->OnlineSessionsConfigSpec.SessionCreateSettings.NumPublicConnections);
			AllowJoinInProgressComboString->SetSelectedIndex(static_cast<int32>(InConfig->OnlineSessionsConfigSpec.SessionCreateSettings.bAllowJoinInProgress));
		}
		UGLCOnlineSessionsLibrary::InitComboBox(GameModeComboBox);
	}

	bInit = true;
}

void UUI_ConfigSet::CancelButtonPress()
{
	if(UUI_MenuMain* InMain = GetFather<>())
	{
		InMain->PlayConfigOutAnim();
	}
}

void UUI_ConfigSet::OnRootPasswordTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	ModifyConfig();
}

void UUI_ConfigSet::OnUI_PasswordComboChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if(!UI_PasswordCombo) return;

	int index = UI_PasswordCombo->GetSelectedIndex();
	PasswordHorizontalBox_2->SetIsEnabled((bool)index);
}

void UUI_ConfigSet::OnRootNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	ModifyConfig();
}

void UUI_ConfigSet::OnGameModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	ModifyConfig();
}

void UUI_ConfigSet::OnPlayerNumCommitted(float InValue, ETextCommit::Type CommitMethod)
{
	ModifyConfig();
}

void UUI_ConfigSet::OnAllowJoinInProgressComboStringChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	ModifyConfig();
}

void UUI_ConfigSet::ModifyConfig()
{
	if(!bInit) return;

	if(RootNameText && GetGameModeComboBox() && MaxPlayerNum && GetAllowJoinInProgressComboString())
	{
		if (UGLCOnlineSessionsConfig* InConfig = UGLCOnlineSessionsLibrary::GetOnlineSessionsConfigFromSlot())
		{
			InConfig->OnlineSessionsConfigSpec.SessionCreateSettings.bAllowJoinInProgress = static_cast<bool>(AllowJoinInProgressComboString->GetSelectedIndex());
			InConfig->OnlineSessionsConfigSpec.SessionCreateSettings.MapID = GameModeComboBox->GetSelectedIndex();
			InConfig->OnlineSessionsConfigSpec.SessionCreateSettings.NumPublicConnections = static_cast<int32>(MaxPlayerNum->GetValue());
			InConfig->OnlineSessionsConfigSpec.SessionCreateSettings.RoomName = RootNameText->GetText().ToString();
			InConfig->OnlineSessionsConfigSpec.SessionCreateSettings.RoomPassword = RoomPassword->GetText().ToString();
			InConfig->OnlineSessionsConfigSpec.SessionCreateSettings.bPassword = static_cast<bool>(UI_PasswordCombo->GetSelectedIndex());

			UGLCOnlineSessionsLibrary::SaveOnlineSessionsConfigToSlot(InConfig);
		}
	}
}
