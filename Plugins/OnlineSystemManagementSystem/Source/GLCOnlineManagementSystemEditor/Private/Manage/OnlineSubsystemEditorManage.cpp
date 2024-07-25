#include "Manage/OnlineSubsystemEditorManage.h"
#include <Misc/FileHelper.h>



FOnlineSubsystemEditorManage* FOnlineSubsystemEditorManage::EditorManage = nullptr;
const FString FOnlineSubsystemEditorManage::DeFaultEnginePath = FPaths::ProjectConfigDir() / TEXT("DefaultEngine.ini");


FOnlineSubsystemEditorManage::FOnlineSubsystemEditorManage()
{
	PlatformState = EPlatformState::Null;
}

EPlatformState FOnlineSubsystemEditorManage::GetPlatformState()
{
	return PlatformState;
}

int32 FOnlineSubsystemEditorManage::GetAppID()
{
	TArray<FString> NewStrings;

	LoadDefaultEngine(NewStrings);
	if (NewStrings.Num())
	{
		if (NewStrings.Contains(TEXT("[OnlineSubsystem]")))
		{
			for (auto& Tem : NewStrings)
			{
				if (Tem.Contains(TEXT("SteamDevAppId")))
				{
					FString Left;
					FString Right;
					if (Tem.Split(TEXT("="), &Left, &Right))
					{
						return FCString::Atoi(*Right);
					}
				}
			}
		}
	}
	return INDEX_NONE;
}

void FOnlineSubsystemEditorManage::InitSteam()
{
	CheckAndDelete();
	SaveAppID(480);
}

void FOnlineSubsystemEditorManage::SaveAppID(const int32 InAppID)
{
	TArray<FString> NewStrings;

	if (FFileHelper::LoadFileToStringArray(NewStrings, *DeFaultEnginePath))
	{
		if (NewStrings.Num())
		{
			if (NewStrings.Contains(TEXT("[OnlineSubsystemSteam]")))
			{
				for (auto& Tem : NewStrings)
				{
					if (Tem.Contains(TEXT("SteamDevAppId")))
					{
						Tem = FString::Printf(TEXT("SteamDevAppId=%d"), InAppID);
						break;
					}
				}
			}
			else
			{
				CheckAndDelete();
				NewStrings.Add(TEXT(""));
				NewStrings.Add(TEXT(""));
				NewStrings.Add(TEXT("[/Script/Engine.GameEngine]"));
				NewStrings.Add(TEXT("+NetDriverDefinitions=(DefName=\"GameNetDriver\",DriverClassName=\"OnlineSubsystemSteam.SteamNetDriver\",DriverClassNameFallback=\"OnlineSubsystemUtils.IpNetDriver\")"));
				NewStrings.Add(TEXT(""));
				NewStrings.Add(TEXT("[OnlineSubsystem]"));
				NewStrings.Add(TEXT("DefaultPlatformService=Steam"));
				NewStrings.Add(TEXT(""));
				NewStrings.Add(TEXT("[OnlineSubsystemSteam]"));
				NewStrings.Add(TEXT("bEnabled=true"));
				NewStrings.Add(FString::Printf(TEXT("SteamDevAppId=%d"), InAppID));
				NewStrings.Add(TEXT("bInitServerOnClient=true"));
				NewStrings.Add(TEXT(""));
				NewStrings.Add(TEXT("[/Script/OnlineSubsystemSteam.SteamNetDriver]"));
				NewStrings.Add(TEXT("NetConnectionClassName=\"OnlineSubsystemSteam.SteamNetConnection\""));
			}


			IFileManager::Get().Delete(*DeFaultEnginePath);
			FFileHelper::SaveStringArrayToFile(NewStrings, *DeFaultEnginePath);
		}
	}
}

void FOnlineSubsystemEditorManage::InitEOSForEpic()
{
	PlatformState = EPlatformState::EOSForEpic;
}

void FOnlineSubsystemEditorManage::InitEOSForEpic_Exe()
{
	CheckAndDelete();
	TArray<FString> NewStrings;
	if (LoadDefaultEngine(NewStrings))
	{
		NewStrings.Add(FString());
		NewStrings.Add(TEXT("[OnlineSubsystemEOS]"));
		NewStrings.Add(TEXT("bEnabled=true"));
		NewStrings.Add(FString());
		NewStrings.Add(TEXT("[OnlineSubsystem]"));
		NewStrings.Add(TEXT("DefaultPlatformService=EOS"));
		NewStrings.Add(FString());
		NewStrings.Add(TEXT("[/Script/Engine.GameEngine]"));
		NewStrings.Add(TEXT("+NetDriverDefinitions=(DefName=\"GameNetDriver\",DriverClassName=\"OnlineSubsystemEOS.NetDriverEOS\",DriverClassNameFallback=\"OnlineSubsystemUtils.IpNetDriver\")"));
		NewStrings.Add(FString());
		NewStrings.Add(TEXT("[/Script/OnlineSubsystemEOS.NetDriverEOS]"));
		NewStrings.Add(TEXT("bIsUsingP2PSockets=true"));

		FFileHelper::SaveStringArrayToFile(NewStrings, *DeFaultEnginePath);
	}
}

void FOnlineSubsystemEditorManage::InitEOSForSteam()
{
	PlatformState = EPlatformState::EOSForSteam;
}

void FOnlineSubsystemEditorManage::InitEOSForSteam_Exe()
{
	CheckAndDelete();
	TArray<FString> NewStrings;
	if (LoadDefaultEngine(NewStrings))
	{
		NewStrings.Add(FString());
		NewStrings.Add(FString("[OnlineSubsystemEOSPlus]"));
		NewStrings.Add(FString("bEnabled=true"));
		NewStrings.Add(FString());
		NewStrings.Add(FString("[OnlineSubsystemEOS]"));
		NewStrings.Add(FString("bEnabled=true"));
		NewStrings.Add(FString());
		NewStrings.Add(FString("[OnlineSubsystemSteam]"));
		NewStrings.Add(FString("bEnabled=true"));
		/*NewStrings.Add(FString("bUseSteamNetworking=false"));*/
		NewStrings.Add(FString("bInitServerOnClient=true"));
		NewStrings.Add(FString("SteamDevAppId=480"));
		NewStrings.Add(FString());
		NewStrings.Add(FString("[OnlineSubsystem]"));
		NewStrings.Add(FString("DefaultPlatformService=EOSPlus"));
		NewStrings.Add(FString("NativePlatformService=Steam"));
		NewStrings.Add(FString());
		NewStrings.Add(FString("[/Script/OnlineSubsystemUtils.OnlineEngineInterfaceImpl]"));
		NewStrings.Add(FString("+CompatibleUniqueNetIdTypes=EOS"));
		NewStrings.Add(FString("+CompatibleUniqueNetIdTypes=EOSPlus"));
		NewStrings.Add(FString());
		NewStrings.Add(TEXT("[/Script/Engine.GameEngine]"));
		NewStrings.Add(TEXT("+NetDriverDefinitions=(DefName=\"GameNetDriver\",DriverClassName=\"OnlineSubsystemEOS.NetDriverEOS\",DriverClassNameFallback=\"OnlineSubsystemUtils.IpNetDriver\")"));
		NewStrings.Add(TEXT("+NetDriverDefinitions=(DefName=\"GameNetDriver\",DriverClassName=\"OnlineSubsystemSteam.SteamNetDriver\",DriverClassNameFallback=\"OnlineSubsystemUtils.IpNetDriver\")"));
		NewStrings.Add(FString());
		NewStrings.Add(TEXT("[/Script/OnlineSubsystemEOS.NetDriverEOS]"));
		NewStrings.Add(TEXT("bIsUsingP2PSockets=true"));

		FFileHelper::SaveStringArrayToFile(NewStrings, *DeFaultEnginePath);
	}
}

bool FOnlineSubsystemEditorManage::LoadDefaultEngine(TArray<FString>& OutString)
{
	OutString.Empty();
	if (FFileHelper::LoadFileToStringArray(OutString, *DeFaultEnginePath))
	{

	}
	return OutString.Num() > 0;
}

void FOnlineSubsystemEditorManage::CheckAndDelete()
{
	FString FilePath = FPaths::ProjectConfigDir() + TEXT("DefaultEngine.ini");

	if (!FPaths::FileExists(FilePath))
	{
		return;
	}

	FString FileContent;

	//Read file
	if (FFileHelper::LoadFileToString(FileContent, *FilePath))
	{
		bool bNeedSave = false;

		int32 StartIndex = FileContent.Find(TEXT("[OnlineSubsystemEOSPlus]"));
		int32 EndIndex = FileContent.Find(TEXT("bIsUsingP2PSockets=true"))
			+ FString(TEXT("bIsUsingP2PSockets=true")).Len();
		if (StartIndex >= 0 && EndIndex > 0)
		{
			FileContent.RemoveAt(StartIndex, EndIndex - StartIndex, false);
			bNeedSave = true;
		}

		StartIndex = FileContent.Find(TEXT("[OnlineSubsystemEOS]"));
		EndIndex = FileContent.Find(TEXT("bIsUsingP2PSockets=true")) + FString(TEXT("bIsUsingP2PSockets=true")).Len();
		if (StartIndex >= 0 && EndIndex > 0)
		{
			FileContent.RemoveAt(StartIndex, EndIndex - StartIndex, false);
			bNeedSave = true;
		}

		StartIndex = FileContent.Find(TEXT("[/Script/Engine.GameEngine]"));
		EndIndex = FileContent.Find(TEXT("NetConnectionClassName=\"OnlineSubsystemSteam.SteamNetConnection\"")) + FString(TEXT("NetConnectionClassName=\"OnlineSubsystemSteam.SteamNetConnection\"")).Len();
		if (StartIndex >= 0 && EndIndex > 0)
		{
			FileContent.RemoveAt(StartIndex, EndIndex - StartIndex, false);
			bNeedSave = true;
		}

		if (bNeedSave)
		{
			FFileHelper::SaveStringToFile(FileContent, *FilePath);
		}
	}
}

bool FOnlineSubsystemEditorManage::InitVoiceChat()
{
	if(IsVoiceChatInit()) return false;

	TArray<FString> EngineStrings;
	if (LoadDefaultEngine(EngineStrings))
	{
		EngineStrings.Add(TEXT(""));
		EngineStrings.Add(TEXT("[OnlineSubsystem]"));
		EngineStrings.Add(TEXT("bHasVoiceEnabled=true"));
		EngineStrings.Add(TEXT("[Voice]"));
		EngineStrings.Add(TEXT("bEnabled=true"));

		FString MyFileName = FPaths::ProjectConfigDir() / TEXT("DefaultGame.ini");
		TArray<FString> GameStrings;
		FFileHelper::LoadFileToStringArray(GameStrings, *MyFileName);

		if (GameStrings.Num())
		{
			bool bContain = false;
			for (auto& Temp : GameStrings)
			{
				if (Temp.Contains(TEXT("bRequiresPushToTalk")))
				{
					bContain = true;
					break;
				}
			}

			FFileHelper::SaveStringArrayToFile(EngineStrings, *DeFaultEnginePath);

			if (!bContain)
			{
				GameStrings.Add(TEXT(""));
				GameStrings.Add(TEXT("[/Script/Engine.GameSession]"));
				GameStrings.Add(TEXT("bRequiresPushToTalk=true"));
				FFileHelper::SaveStringArrayToFile(GameStrings, *MyFileName);
			}
			return true;
		}
	}
	return false;
}

bool FOnlineSubsystemEditorManage::IsVoiceChatInit()
{
	TArray<FString> EngineStrings;
	if (LoadDefaultEngine(EngineStrings))
	{
		for (auto& Temp : EngineStrings)
		{
			if (Temp.Contains(TEXT("bHasVoiceEnabled")))
			{
				return true;
			}
		}
	}
	return false;
}

bool FOnlineSubsystemEditorManage::IsCurrentSteam()
{
	TArray<FString> NewString;
	if (LoadDefaultEngine(NewString))
	{
		for (auto& Temp : NewString)
		{
			if (Temp.Contains("SteamDevAppId"))
			{
				return true;
			}
		}
	}
	return false;
}

bool FOnlineSubsystemEditorManage::IsCurrentEOSForSteam()
{
	bool bIsEOS = false;
	bool bContainSteam = false;
	TArray<FString> NewString;
	if (LoadDefaultEngine(NewString))
	{
		for (auto& Temp : NewString)
		{
			if (Temp.Contains(TEXT("NativePlatformService=Steam")))
			{
				bContainSteam = true;
			}
			else if (Temp.Contains(TEXT("[OnlineSubsystemEOS]")))
			{
				bIsEOS = true;
			}
		}
	}
	return bIsEOS && bContainSteam;
}

bool FOnlineSubsystemEditorManage::IsCurrentEOSForEpic()
{
	bool bIsEOS = false;
	bool bContainSteam = false;
	TArray<FString> NewString;
	if (LoadDefaultEngine(NewString))
	{
		for (auto& Temp : NewString)
		{
			if (Temp.Contains(TEXT("NativePlatformService=Steam")))
			{
				bContainSteam = true;
			}
			else if (Temp.Contains(TEXT("[OnlineSubsystemEOS]")))
			{
				bIsEOS = true;
			}
		}
	}
	return bIsEOS && !bContainSteam;
}

FOnlineSubsystemEditorManage* FOnlineSubsystemEditorManage::Get()
{
	if (!EditorManage)
	{
		EditorManage = new FOnlineSubsystemEditorManage();
	}
	return EditorManage;
}

void FOnlineSubsystemEditorManage::Destroy()
{
	if (EditorManage)
	{
		delete EditorManage;
	}
	EditorManage = nullptr;
}
