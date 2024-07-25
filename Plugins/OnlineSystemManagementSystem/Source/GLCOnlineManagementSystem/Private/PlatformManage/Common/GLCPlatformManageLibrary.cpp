#include "PlatformManage/Common/GLCPlatformManageLibrary.h"
#include "Common/DataTable/CommonSettings.h"
#include "OnlineSubsystem.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Engine/LocalPlayer.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerState.h"
#include "Common/GLCGameInstance.h"
#include "Common/Components/GLCOnlineManageComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "Misc\Paths.h"
#include "Misc\SecureHash.h"
#include "Misc\AES.h"
 

UGLCPlatformManageLibrary::UGLCPlatformManageLibrary(const FObjectInitializer& Initializer)
{

}

ANSICHAR* UGLCPlatformManageLibrary::GetHashKey(const FString& InFileName)
{
	checkf(!InFileName.IsEmpty(),TEXT(" 'InFileName' is empty !!! You can not pass a empty file name !!!"))

	FString CopyFileName = InFileName;
	FPaths::GetBaseFilename(CopyFileName);
	FString HashKey = FMD5::HashAnsiString(*InFileName);

	for (int32 i = 0; i < 32; i++)
	{
		HashKey = FMD5::HashAnsiString(*HashKey);
	}
	return TCHAR_TO_UTF8(*HashKey);
}

void UGLCPlatformManageLibrary::MemoryToMultipliesOfSixteen(TArray<uint8>& OutMemory)
{
	while (OutMemory.Num() % 16 != 0)
	{
		uint8 Temp = 0;
		OutMemory.Add(Temp);
	}
}

UWorld* UGLCPlatformManageLibrary::GetWorldFormObject(UObject* InObject)
{
	if(GEngine && IsValid(InObject))
	if(GEngine && IsValid(InObject))
	{
		return GEngine->GetWorldFromContextObject(InObject,EGetWorldErrorMode::LogAndReturnNull);
	}
	return nullptr;
}

void UGLCPlatformManageLibrary::K2_MD5EncryptionFromFStringToMemory(const FString& InContent, 
                                                                    const FString& InFileName, TArray<uint8>& OutMemory)
{
	FTCHARToUTF8 Datas(*InContent);
	OutMemory.Append((uint8*)Datas.Get(),Datas.Length());
	MemoryToMultipliesOfSixteen(OutMemory);

	FAES::EncryptData(OutMemory.GetData(), OutMemory.Num(),GetHashKey(InFileName));
}

UGameInstance* UGLCPlatformManageLibrary::GetGameInstance()
{
	if(APlayerController* InPC = GetPlayerController())
	{
		if(InPC->GetWorld())
		{
			return InPC->GetWorld()->GetGameInstance();
		}
	}
	return nullptr;
}

void UGLCPlatformManageLibrary::K2_MD5EncryptionFromMemoryToMemory(const FString& InFileName, 
                                                                   TArray<uint8>& OutMemory)
{
	MemoryToMultipliesOfSixteen(OutMemory);

	FAES::EncryptData(OutMemory.GetData(), OutMemory.Num(), GetHashKey(InFileName));
}

void UGLCPlatformManageLibrary::K2_MD5EncryptionFormFStringArrayToMemory(TArray<FString> InStrings, 
	const FString& InFileName, TArray<FFileEncryptionData>& OutMenmorys)
{
	for (auto& Temp : InStrings)
	{
		FFileEncryptionData& InData = OutMenmorys.AddDefaulted_GetRef();
		K2_MD5EncryptionFromFStringToMemory(Temp,InFileName, InData.SingleDatas);
	}
}

void UGLCPlatformManageLibrary::K2_MD5DecryptFromMemoryToFString(TArray<uint8> InBytes, const FString& InFileName, 
	FString& OutString)
{
	MemoryToMultipliesOfSixteen(InBytes);

	FAES::DecryptData(InBytes.GetData(), InBytes.Num(), GetHashKey(InFileName));

	FUTF8ToTCHAR Data((const ANSICHAR*)InBytes.GetData());
	OutString.Append(Data.Get(),Data.Length());
}

void UGLCPlatformManageLibrary::K2_MD5DecryptFromMemoryToMemory(const FString& InFileName, TArray<uint8>& OutMemory)
{
	MemoryToMultipliesOfSixteen(OutMemory);
	FAES::DecryptData(OutMemory.GetData(),OutMemory.Num(),GetHashKey(InFileName));
}

void UGLCPlatformManageLibrary::K2_MD5DecryptFromMemoryToStringArray(TArray<FFileEncryptionData> InMemorys,
	const FString& InFileName, TArray<FString>& OutStrings)
{
	for (auto& Temp : InMemorys)
	{
		FString& InString = OutStrings.AddDefaulted_GetRef();
		K2_MD5DecryptFromMemoryToFString(Temp.SingleDatas, InFileName,InString);
	}
}

bool UGLCPlatformManageLibrary::RegisterGLCOnlineManagerComponentFromController(APlayerController* InPlayerController)
{
	if (IsValid(InPlayerController) && InPlayerController->IsLocalController())
	{
		FOnlineSubsystemDataManage::Get()->SetMyPlayerController(InPlayerController);
		if (UGLCOnlineManageComponent* InManageCom = GetGLCOnlineManageComponentFormObject(InPlayerController))
		{
			PreInit();

			FOnlineSubsystemDataManage::Get()->SetOnlineManageComponent(InManageCom);
			return true;
		}
	}
	return false;
}

void UGLCPlatformManageLibrary::PreInit()
{
	const FString MyFileName1 = TEXT("MyFileName.123");
	FString Test1 = TEXT("123");
	TArray<uint8> Test1Datas;
	UGLCPlatformManageLibrary::K2_MD5EncryptionFromFStringToMemory(Test1, MyFileName1, Test1Datas);

	FUTF8ToTCHAR Data((const ANSICHAR*)Test1Datas.GetData());
	FString TestKey;
	TestKey.AppendChars(Data.Get(), Data.Length());

	Test1.Empty();
	UGLCPlatformManageLibrary::K2_MD5DecryptFromMemoryToFString(Test1Datas, MyFileName1, Test1);
}

UGLCOnlineManageComponent* UGLCPlatformManageLibrary::GetGLCOnlineManagerComponent()
{
	if (UGLCOnlineManageComponent* InCom = FOnlineSubsystemDataManage::Get()->GetOnlineManageComponent())
	{
		return InCom;
	}
	return nullptr;
}

APlayerController* UGLCPlatformManageLibrary::GetPlayerController()
{
	return FOnlineSubsystemDataManage::Get()->GetMyPlayerController();
}

bool UGLCPlatformManageLibrary::GetUniqueNetIDFromObject(UObject* InRequestObject, 
	FUniqueNetIdRepl& OutNetID)
{
	if (IsValid(InRequestObject))
	{
		if (UWorld* InWorld = GEngine->GetWorldFromContextObject(InRequestObject,EGetWorldErrorMode::LogAndReturnNull))
		{
			if (ULocalPlayer* InLocalPlayer = InWorld->GetFirstLocalPlayerFromController())
			{
				OutNetID =  InLocalPlayer->GetPreferredUniqueNetId();
				return true;
			}
		}
	}

	return false;
}
                                                                                         
bool UGLCPlatformManageLibrary::GetUniqueNetID(FUniqueNetIdRepl& OutNetID)
{
	return GetUniqueNetIDFromObject(FOnlineSubsystemDataManage::Get()->GetOnlineManageComponent(),OutNetID);
}

UGLCOnlineManageComponent* UGLCPlatformManageLibrary::GetGLCOnlineManageComponentFormObject(UObject* InObject)
{
	if (AActor* InActor = Cast<AActor>(InObject))
	{ 
		return InActor->FindComponentByClass<UGLCOnlineManageComponent>();
	}
	return nullptr;
}

FString UGLCPlatformManageLibrary::GetUserID()
{
	FUniqueNetIdRepl NetID;
	if (FOnlineSubsystemDataManage::Get()->GetUniqueID(NetID))
	{
		return NetID.ToString();
	}
	return FString();
}

FString UGLCPlatformManageLibrary::GetPlatformID()
{
	return IOnlineSubsystem::Get()->GetAppId(); 
}

void UGLCPlatformManageLibrary::GetAllPlayerInfo_OnlyPNAndNetID(TArray<FGLCPlayerInfo>& OutInfos)
{
	if (APlayerController* InPc = GetPlayerController())
	{
		if (UWorld* InWorld = InPc->GetWorld())
		{
			for (TActorIterator<APlayerState> PS(InWorld); PS; ++PS)
			{
				if (PS && PS->GetUniqueId().IsValid() && PS->CheckStillInWorld())
				{
					FGLCPlayerInfo& NewInfo = OutInfos.AddDefaulted_GetRef();
					NewInfo.NetID = PS->GetUniqueId();
					NewInfo.PlayerName = PS->GetPlayerName();
					NewInfo.PlayerState = *PS;
				}
			}
		}
	}
}

void UGLCPlatformManageLibrary::GetAllPlayerInfo(TArray<FGLCPlayerInfo>& OutInfos,bool bGetSelfInfo)
{
	if (UGLCGameInstance* InGI = GetGLCGameInstance())
	{
		InGI->GetAllPlayerInfo(OutInfos,bGetSelfInfo);
	}
}

FGLCPlayerInfo UGLCPlatformManageLibrary::GetSelfInfo()
{
	FGLCPlayerInfo NewInfo;
	if (APlayerController* InPC = GetPlayerController())
	{
		if (APlayerState* InPS = InPC->PlayerState)
		{
			TArray<FGLCPlayerInfo> PlayerInfos;
			GetAllPlayerInfo(PlayerInfos);

			for (auto& Temp : PlayerInfos)
			{
				if (Temp.NetID == InPS->GetUniqueId())
				{
					NewInfo == Temp;
					break;
				}
			}
		}
	}
	
	return NewInfo;
}

bool UGLCPlatformManageLibrary::GetPlayerInfoByPlayerName(const FString& InPlayerName, FGLCPlayerInfo& OutInfo)
{
	TArray<FGLCPlayerInfo> PlayerInfos;
	GetAllPlayerInfo(PlayerInfos);

	for (auto& Temp : PlayerInfos)
	{
		if (Temp.PlayerName == InPlayerName)
		{
			OutInfo = Temp;
			return true;
		}
	}
	return false;
}

bool UGLCPlatformManageLibrary::GetPlayerInfoByNetID(const FUniqueNetIdRepl& InNetID, FGLCPlayerInfo& OutInfo)
{
	TArray<FGLCPlayerInfo> PlayerInfos;
	GetAllPlayerInfo(PlayerInfos);

	for (auto& Temp : PlayerInfos)
	{
		if (Temp.NetID == InNetID)
		{
			OutInfo = Temp;
			return true;
		}
	}
	return false;
}

FString UGLCPlatformManageLibrary::UserIDToFString(const FUniqueNetIdRepl& InNetID)
{
	return InNetID->ToString();
}

UGLCGameInstance* UGLCPlatformManageLibrary::GetGLCGameInstance()
{
	if (APlayerController* InPC = GetPlayerController())
	{
		if (UWorld* InWorld = InPC->GetWorld())
		{
			checkf(InWorld->GetGameInstance<UGLCGameInstance>(), TEXT("Get GLCGameInstance fail! Please set GLCGameInstance in your project!"));

			return InWorld->GetGameInstance<UGLCGameInstance>();
		}
	}
	return nullptr;
}

bool UGLCPlatformManageLibrary::IsPlayerInfoEqual(const FGLCPlayerInfo& InLInfo, const FGLCPlayerInfo& InRInfo)
{
	return InLInfo == InRInfo;
}

bool UGLCPlatformManageLibrary::IsNetIDEqual(const FUniqueNetIdRepl& InLID, const FUniqueNetIdRepl& InRID)
{
	return InLID == InRID;
}

