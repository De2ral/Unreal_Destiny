#pragma once
#include "CoreMinimal.h"

enum class EPlatformState
{
	Null,
	EOSForSteam,
	EOSForEpic
};

struct GLCONLINEMANAGEMENTSYSTEMEDITOR_API FOnlineSubsystemEditorManage
{
protected:
	EPlatformState PlatformState;

public:
	FOnlineSubsystemEditorManage();
	
	EPlatformState GetPlatformState();
	int32 GetAppID();

	//Steam
	void InitSteam();
	void SaveAppID(const int32 InAppID);

	//Epic
	void InitEOSForEpic();
	void InitEOSForEpic_Exe();

	//Steam eos
	void InitEOSForSteam();
	void InitEOSForSteam_Exe();
	bool LoadDefaultEngine(TArray<FString>& OutString);

	//Check
	void CheckAndDelete();

	//Voice chat
	bool InitVoiceChat();
	bool IsVoiceChatInit();

	bool IsCurrentSteam();
	bool IsCurrentEOSForSteam();
	bool IsCurrentEOSForEpic();

	static FOnlineSubsystemEditorManage* Get();
	static void Destroy();
protected:
	static FOnlineSubsystemEditorManage* EditorManage;
	static const FString DeFaultEnginePath;
};
