#include "Slate/SInitPannel.h"
#include <Misc/FileHelper.h>
#include "Widgets/Input/SEditableTextBox.h"
#include "HAL/FileManager.h"
#include "Manage/OnlineSubsystemEditorManage.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"


#define LOCTEXT_NAMESPACE "SSInitPannel"


#define SteamName TEXT("steam")
#define EOSForSteam TEXT("EOSForSteam")
#define EpicName TEXT("EOSForEpic")

void SSInitPannel::Construct(const FArguments& InArgs)
{
	PlatformNames.Add(MakeShareable(new FName(SteamName)));
	PlatformNames.Add(MakeShareable(new FName(EOSForSteam)));
	PlatformNames.Add(MakeShareable(new FName(EpicName)));

	ChildSlot
	[
		SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
		[
			SNew(SHorizontalBox)
			
			+ SHorizontalBox::Slot()
			.HAlign(EHorizontalAlignment::HAlign_Left)
			.Padding(2, 4, 2, 4)
			.AutoWidth()
			[
				SAssignNew(PlatformCombo, SComboBox<FComboItemType>)
				.OptionsSource(&PlatformNames)
				.InitiallySelectedItem(GetSelectedPlatformItemFromConfig())
				.OnGenerateWidget(this, &SSInitPannel::MakeWidgetForOtion)
				[
					SNew(STextBlock)
					.Text_Lambda([&]()->FText
					{
						FText RetText;
						if(PlatformCombo.IsValid()) RetText = FText::FromName(*PlatformCombo->GetSelectedItem());
						return RetText;
					})
				]
			]

			+ SHorizontalBox::Slot()
			.HAlign(EHorizontalAlignment::HAlign_Left)
			.Padding(145, 4, 2, 4)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("APPID","AppID"))
			]

			+SHorizontalBox::Slot()
			.HAlign(EHorizontalAlignment::HAlign_Left)
			.AutoWidth()
			.Padding(66, 4, 2, 4)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("PATH TO RELEASE PACKAGED VERSION", "PathToReleasePackagedVersion"))
			]
		]

			+ SVerticalBox::Slot()
			.AutoHeight()
		[
			SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.Padding(2, 4, 2, 4)
				.HAlign(EHorizontalAlignment::HAlign_Left)
				.AutoWidth()
			[
				SNew(SButton)
				.Text(LOCTEXT("INITSTEAM", "InitSteam"))
				.ToolTipText(LOCTEXT("INITBUTTONTIP","If the project is not initialized, initialize it"))
				.OnReleased(this, &SSInitPannel::OnInitSteamButtonPress)
				.HAlign(EHorizontalAlignment::HAlign_Left)
				.IsEnabled(this,&SSInitPannel::CanEnableSteam)
			]

				+ SHorizontalBox::Slot()
				.Padding(125,4,2,4)
				.HAlign(EHorizontalAlignment::HAlign_Left)
				.AutoWidth()
			[
				SAssignNew(AppIDEditableText, SEditableTextBox)
				.IsEnabled(this, &SSInitPannel::CanEnableSteam)
				.Text(FText::FromString(FString::FromInt(GetAppID())))
				.OnTextCommitted(this,&SSInitPannel::OnAppIDTextCommitted)
				.ToolTipText(LOCTEXT("EDIT_YOUR_APPID_HERE","Edit your AppID here"))
			]

				+ SHorizontalBox::Slot()
				.Padding(60, 4, 2, 4)
				.HAlign(EHorizontalAlignment::HAlign_Left)
				.AutoWidth()
			[
				SAssignNew(ProjectPathText, SEditableTextBox)
				.ToolTipText(LOCTEXT("PATH TO RELEASE PACKAGED VERSION Tool Tip", "Path to release packaged version"))
				.IsEnabled(this, &SSInitPannel::CanEnableSteam)
			]

				+SHorizontalBox::Slot()
				.Padding(2, 4, 2, 4)
				.HAlign(EHorizontalAlignment::HAlign_Left)
				.AutoWidth()
			[
				SNew(SButton)
				.Text(LOCTEXT("GenerateApp_idTxt","GenerateApp_idTxt"))
				.OnReleased(this,&SSInitPannel::OnGenerateAppIDTxtButtonPress)
				.IsEnabled(this, &SSInitPannel::CanEnableSteam)
			]
		]
			
			//Init EOSForSteam
			+ SVerticalBox::Slot()
			.Padding(2, 4, 2, 4)
			.HAlign(EHorizontalAlignment::HAlign_Left)
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.Padding(2, 4, 2, 4)
				.HAlign(EHorizontalAlignment::HAlign_Left)
				.AutoWidth()
				[
					SNew(SButton)
					.ToolTipText(LOCTEXT("InitButtonTip", "If the project is not initialized, initialize it"))
					.Text(LOCTEXT("INITEOSFORSTEAM", "InitEOSForSteam"))
					.IsEnabled_Lambda(
						[&]()->bool
					{
						return GetSelectedPlatform() == EPlatform::EOSFORSTEAM;
					})
					.OnReleased_Lambda(
						[&]()
					{
						FOnlineSubsystemEditorManage::Get()->InitEOSForSteam();
						FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("INITIALIZE_SUCCESSFULLY", "Initialize successfully"));
					})
				]
			]

			+SVerticalBox::Slot()
			.Padding(2, 4, 2, 4)
			.HAlign(EHorizontalAlignment::HAlign_Left)
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+SHorizontalBox::Slot()
				.Padding(2, 4, 2, 4)
				.HAlign(EHorizontalAlignment::HAlign_Left)
				.AutoWidth()
				[
					SNew(SButton)
					.ToolTipText(LOCTEXT("InitButtonTip", "If the project is not initialized, initialize it"))
					.Text(LOCTEXT("INITEOSFOREPIC","InitEOSForEpic"))
					.IsEnabled(this,&SSInitPannel::CanEnableEOSForEpic)
					.OnReleased(this,&SSInitPannel::InitEOSButtonPress)
				]
			]

				+ SVerticalBox::Slot()
				.Padding(2, 4, 2, 4)
				.HAlign(EHorizontalAlignment::HAlign_Left)
				.AutoHeight()
				[
					SNew(SHorizontalBox)


				//Voice chat
				+ SHorizontalBox::Slot()
				.Padding(2, 4, 2, 4)
				.HAlign(EHorizontalAlignment::HAlign_Left)
				.AutoWidth()
				[
					SNew(SButton)
					.ToolTipText(LOCTEXT("InitVoice", "Init Voice chat"))
					.Text(LOCTEXT("INITVOICECHAT", "InitVoiceChat"))
					.OnReleased_Lambda([&]()
					{
						if (FOnlineSubsystemEditorManage::Get()->InitVoiceChat())
						{
							FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("INIT_VOICE_CHAT_SUCCESSFUL", "Init voice chat successful"));
						}
						else
						{
							FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("INIT_VOICE_CHAT_FAIL", "Init voice chat fail.It may had been initialized"));
						}
					})
				]
			

			]
	];
}

void SSInitPannel::OnInitSteamButtonPress()
{
	FOnlineSubsystemEditorManage::Get()->InitSteam();
	if (AppIDEditableText)
	{
		AppIDEditableText->SetText(FText::FromString(FString::FromInt(480)));
	}
	FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("INITIALIZE_SUCCESSFULLY", "Initialize successfully"));
}

void SSInitPannel::OnAppIDTextCommitted(const FText& InText, ETextCommit::Type InType)
{
	if (FOnlineSubsystemEditorManage::Get()->IsCurrentSteam())
	{
		SaveAppID(FCString::Atoi(*InText.ToString()));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("MUST INIT STEAM FIRST", "Must Init steam first"));
	}
}

int32 SSInitPannel::GetAppID()
{
	return FOnlineSubsystemEditorManage::Get()->GetAppID();
}

void SSInitPannel::SaveAppID(const int32 InAppID)
{
	FOnlineSubsystemEditorManage::Get()->SaveAppID(InAppID);
}

void SSInitPannel::OnGenerateAppIDTxtButtonPress()
{
	if (ProjectPathText && AppIDEditableText)
	{
		if (ProjectPathText->GetText().IsEmpty())
		{
			FMessageDialog::Open(EAppMsgType::Ok,LOCTEXT("MUSH EDIT PROJECTPATH FIRST","Must edit ProjectPath first"));
		}
		else
		{
			FString OldPath = ProjectPathText->GetText().ToString();
			FPaths::NormalizeFilename(OldPath);

			if (IFileManager::Get().DirectoryExists(*OldPath))
			{
				TArray<FString> StringToSave;
				StringToSave.Add(FString::Printf(TEXT("%d"), FCString::Atoi(*AppIDEditableText->GetText().ToString())));

				FString ProjectName = FPaths::GetCleanFilename(FPaths::GetProjectFilePath());
				ProjectName.ReplaceInline(TEXT(".uproject"),TEXT(""));

				FString NewPath = OldPath / ProjectName / TEXT("Binaries") / TEXT("Win64") / TEXT("steam_appid.txt");

				FText CheckDiretory = FText::FromString((OldPath / ProjectName));
				if (IFileManager::Get().DirectoryExists(*CheckDiretory.ToString()))
				{
					FFileHelper::SaveStringArrayToFile(StringToSave, *NewPath);
					FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("GENERATE SUCCESSFUL", "Generate successful"));
				}
				else
				{
					FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("GENERATE FAIL", "[Error] Generate fail , {0} is not exist ! Can not modify your diretory name"), CheckDiretory));
				}
			}
			else
			{
				FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("PATH FAIL", "[Error] Generate fail , {0} is not exist !"), FText::FromString(OldPath)));
			}
		}
	}
}

TSharedRef<SWidget> SSInitPannel::MakeWidgetForOtion(FComboItemType InOption)
{
	return SNew(STextBlock).Text(FText::FromString(InOption->ToString()));
}

bool SSInitPannel::CanEnableSteam() const
{
	EPlatform Platform = GetSelectedPlatform();
	return Platform == EPlatform::STEAM || Platform == EPlatform::EOSFORSTEAM;
}

bool SSInitPannel::CanEnableEOSForEpic() const
{
	return GetSelectedPlatform() == EPlatform::EOSFOREPIC;
}

EPlatform SSInitPannel::GetSelectedPlatform() const
{
	if (PlatformCombo)
	{
		EPlatform Type = EPlatform::Null;
		FComboItemType SelectItem = PlatformCombo->GetSelectedItem();
		if(!SelectItem.IsValid()) return EPlatform::Null;

		if (SelectItem->ToString() == EpicName)
		{
			Type = EPlatform::EOSFOREPIC;
		}
		else if (SelectItem->ToString() == SteamName)
		{
			Type = EPlatform::STEAM;
		}
		else if (SelectItem->ToString() == EOSForSteam)
		{
			Type = EPlatform::EOSFORSTEAM;
		}
		return Type;
	}
	return EPlatform::Null;
}

FComboItemType SSInitPannel::GetSelectedPlatformItemFromConfig() const
{
	if (FOnlineSubsystemEditorManage::Get()->IsCurrentSteam())
	{
		return PlatformNames[0];
	}
	else if (FOnlineSubsystemEditorManage::Get()->IsCurrentEOSForSteam())
	{
		return PlatformNames[1];
	}
	else if (FOnlineSubsystemEditorManage::Get()->IsCurrentEOSForEpic())
	{
		return PlatformNames[2];
	}
	return PlatformNames[1];
}

FText SSInitPannel::GetPlatformSelectedText() const
{
	FComboItemType Item = GetSelectedPlatformItemFromConfig();
	if (!Item->ToString().IsEmpty())
	{
		return FText::FromString(Item->ToString());
	}
	return FText::FromString(SteamName);
}

void SSInitPannel::OnSelectChanged(FComboItemType InItem, ESelectInfo::Type Info)
{
	if (CurrentPlatformTextPtr.IsValid() && InItem)
	{
		CurrentPlatformTextPtr->SetText(FText::FromString(InItem->ToString()));
	}
}

void SSInitPannel::InitEOSButtonPress()
{
	FOnlineSubsystemEditorManage::Get()->InitEOSForEpic();
	FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("INITIALIZE_SUCCESSFULLY", "Initialize successfully"));
}

bool SSInitPannel::CanOpenVoiceChat() const
{
	return FOnlineSubsystemEditorManage::Get()->IsVoiceChatInit();
}

#undef LOCTEXT_NAMESPACE
