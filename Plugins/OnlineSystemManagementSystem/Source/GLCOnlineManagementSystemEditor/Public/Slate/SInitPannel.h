#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SEditableTextBox;

enum class EPlatform : uint8
{
	Null,
	STEAM,
	EOSFORSTEAM,
	EOSFOREPIC
};

typedef TSharedPtr<FName> FComboItemType;

class SSInitPannel :public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSInitPannel)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void OnInitSteamButtonPress();

	void SaveAppID(const int32 InAppID);
	int32 GetAppID();

	void OnAppIDTextCommitted(const FText& InText, ETextCommit::Type InType);

	void OnGenerateAppIDTxtButtonPress();

	TSharedRef<SWidget> MakeWidgetForOtion(FComboItemType InOption);

	bool CanEnableSteam() const;

	bool CanEnableEOSForEpic() const;

	EPlatform GetSelectedPlatform() const;

	FComboItemType GetSelectedPlatformItemFromConfig() const;
	FText GetPlatformSelectedText() const;

	void OnSelectChanged(FComboItemType InItem,ESelectInfo::Type Info);

	void InitEOSButtonPress();

	bool CanOpenVoiceChat() const;

protected:
	TSharedPtr<SEditableTextBox> AppIDEditableText;
	TSharedPtr<SEditableTextBox> DataTablePathEditableText;
	TSharedPtr<SEditableTextBox> ProjectPathText;
	TSharedPtr<SComboBox<FComboItemType>> PlatformCombo;
	TSharedPtr<STextBlock> CurrentPlatformTextPtr;
	TArray<FComboItemType> PlatformNames;
	
};
