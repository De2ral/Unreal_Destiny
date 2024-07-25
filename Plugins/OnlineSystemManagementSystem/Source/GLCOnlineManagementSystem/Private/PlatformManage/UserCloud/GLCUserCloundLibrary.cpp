
#include "PlatformManage/UserClound/GLCUserCloundLibrary.h"
#include <PlatformManage/Common/GLCPlatformManageLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "OnlineSubsystemTypes.h"
#include <PlatformManage/UserClound/UserCloudManage.h>


UGLCUserCloundLibrary::UGLCUserCloundLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

bool UGLCUserCloundLibrary::K2_ReadUserFile(const FString& InFileName, 
	FPlatformManageOptionalParam OptionalParam /*= FPlatformManageOptionalParam()*/)
{
	return ReadUserFile(InFileName,OptionalParam);
}

bool UGLCUserCloundLibrary::ReadUserFile(const FString& InFileName, const FPlatformManageOptionalParam& OptionalParam /*= FPlatformManageOptionalParam()*/)
{
	if (!InFileName.IsEmpty())
	{
		if (UUserCloudManage* InManage = UUserCloudManage::Get())
		{
			return InManage->ReadUserFile(InFileName, OptionalParam);
		}
	}
	return false;
}

bool UGLCUserCloundLibrary::GetFileContentsAsMemory(const FString& FileName, 
	TArray<uint8>& FileContents)
{
	if (!FileName.IsEmpty())
	{
		if (UUserCloudManage* InManage = UUserCloudManage::Get())
		{
			FileContents.Empty();
			return InManage->GetFileContents(FileName, FileContents);
		}
	}
	return false;
}

bool UGLCUserCloundLibrary::GetFileContentsAsString(const FString& FileName, FString& OutString, 
	bool bMD5Decryption /*= true*/)
{
	if (!FileName.IsEmpty())
	{
		TArray<uint8> Datas;
		if (GetFileContentsAsMemory(FileName, Datas))
		{
			OutString.Empty();
			if (bMD5Decryption)
			{
				UGLCPlatformManageLibrary::K2_MD5DecryptFromMemoryToFString(Datas,FileName,OutString);
			}
			else
			{
				FUTF8ToTCHAR Data((const ANSICHAR*)Datas.GetData(),Datas.Num());
				OutString.Append(Data.Get(),Data.Length());
			}
			return true;
		}
	}
	return false;
}

USaveGame* UGLCUserCloundLibrary::GetFileContentsAsSaveGame(const FString& FileName, bool bMD5Decryption)
{
	if (!FileName.IsEmpty())
	{
		TArray<uint8> Datas;
		if (UUserCloudManage* InManage = UUserCloudManage::Get())
		{
			if (InManage->GetFileContents(FileName, Datas))
			{
				if (bMD5Decryption)
				{
					UGLCPlatformManageLibrary::K2_MD5DecryptFromMemoryToMemory(FileName,Datas);
				}
				return UGameplayStatics::LoadGameFromMemory(Datas);
			}
		}
	}
	return nullptr;
}

bool UGLCUserCloundLibrary::K2_WriteUserFileFromMemory(const FString& FileName,
	const TArray<uint8>& FileContents, FPlatformManageOptionalParam OptionalParam /*= FPlatformManageOptionalParam()*/,
	bool bCompressBeforeUpload /*= false*/)
{
	TArray<uint8> Datas = FileContents;

	return WriteUserFileFromMemory(FileName, Datas,OptionalParam,bCompressBeforeUpload);
}

bool UGLCUserCloundLibrary::WriteUserFileFromMemory(const FString& FileName, 
	TArray<uint8>& FileContents, const FPlatformManageOptionalParam& OptionalParam,
	bool bCompressBeforeUpload /*= false*/)
{
	if (!FileName.IsEmpty())
	{
		if (UUserCloudManage* InManage = UUserCloudManage::Get())
		{
			return InManage->WriteUserFile(FileName, FileContents, OptionalParam, bCompressBeforeUpload);
		}
	}
	return false;
}

bool UGLCUserCloundLibrary::K2_WriteUserFileFromString(const FString& FileName, const FString& FileContents, 
	FPlatformManageOptionalParam OptionalParam /*= FPlatformManageOptionalParam()*/, 
	bool bCompressBeforeUpload /*= false*/, bool bMD5Encrypt /*= true*/)
{
	return WriteUserFileFromString(FileName,FileContents,OptionalParam,bCompressBeforeUpload,bMD5Encrypt);
}

bool UGLCUserCloundLibrary::WriteUserFileFromString(const FString& FileName, const FString& FileContents, 
	const FPlatformManageOptionalParam& OptionalParam /*= FPlatformManageOptionalParam()*/, 
	bool bCompressBeforeUpload /*= false*/, bool bMD5Encrypt /*= true*/)
{
	TArray<uint8> Datas;
	if (bMD5Encrypt)
	{
		UGLCPlatformManageLibrary::K2_MD5EncryptionFromFStringToMemory(FileContents, FileName, Datas);
	}
	else
	{
		FTCHARToUTF8 Data(*FileContents,FileContents.Len());
		Datas.Append((uint8*)Data.Get(), Data.Length());
	}

	return WriteUserFileFromMemory(FileName, Datas, OptionalParam, bCompressBeforeUpload);
}

bool UGLCUserCloundLibrary::K2_WriteUserFileFromSaveGame(const FString& FileName, USaveGame* InSaveGame,
	FPlatformManageOptionalParam OptionalParam /*= FPlatformManageOptionalParam()*/, 
	bool bCompressBeforeUpload /*= false*/, bool bMD5Encrypt)
{
	return WriteUserFileFromSaveGame(FileName,InSaveGame,OptionalParam,bCompressBeforeUpload);
}

bool UGLCUserCloundLibrary::WriteUserFileFromSaveGame(const FString& FileName, USaveGame* InSaveGame,
	const FPlatformManageOptionalParam& OptionalParam /*= FPlatformManageOptionalParam()*/, 
	bool bCompressBeforeUpload /*= false*/, bool bMD5Encrypt)
{
	TArray<uint8> Datas;
	if (UGameplayStatics::SaveGameToMemory(InSaveGame,Datas))
	{
		if (bMD5Encrypt)
		{
			UGLCPlatformManageLibrary::K2_MD5EncryptionFromMemoryToMemory(FileName,Datas);
		}
		return WriteUserFileFromMemory(FileName, Datas, OptionalParam, bCompressBeforeUpload);
	}

	return false;
}

bool UGLCUserCloundLibrary::K2_DeleteUserFile(const FString& FileName, 
	FPlatformManageOptionalParam OptionalParam /*= FPlatformManageOptionalParam()*/, 
	bool bShouldCloudDelete /*= true*/, bool bShouldLocallyDelete /*= true*/)
{
	return DeleteUserFile(FileName,OptionalParam,bShouldCloudDelete,bShouldLocallyDelete);
}

bool UGLCUserCloundLibrary::DeleteUserFile(const FString& FileName, 
	const FPlatformManageOptionalParam& OptionalParam /*= FPlatformManageOptionalParam()*/, 
	bool bShouldCloudDelete /*= true*/, bool bShouldLocallyDelete /*= true*/)
{
	if (UUserCloudManage* InManage = UUserCloudManage::Get())
	{
		return InManage->DeleteUserFile(FileName, OptionalParam, bShouldCloudDelete, bShouldLocallyDelete);
	}
	return false;
}

bool UGLCUserCloundLibrary::K2_EnumerateUserFiles(FPlatformManageOptionalParam OptionalParam /*= FPlatformManageOptionalParam()*/)
{
	return EnumerateUserFiles(OptionalParam);
}

bool UGLCUserCloundLibrary::EnumerateUserFiles(const FPlatformManageOptionalParam& OptionalParam /*= FPlatformManageOptionalParam()*/)
{
	if (UUserCloudManage* InManage = UUserCloudManage::Get())
	{
		return InManage->EnumerateUserFiles(OptionalParam);
	}
	return false;
}

bool UGLCUserCloundLibrary::ClearFiles()
{
	if (UUserCloudManage* InManage = UUserCloudManage::Get())
	{
		return InManage->ClearFiles();
	}
	return false;
}

bool UGLCUserCloundLibrary::ClearFile(const FString& InFileName)
{
	if (UUserCloudManage* InManage = UUserCloudManage::Get())
	{
		return InManage->ClearFile(InFileName);
	}
	return false;
}

bool UGLCUserCloundLibrary::K2_GetUserFileLists(TArray<FGLCCloudFileHeader>& OutFileLists)
{
	bool Ret = false;
	if (UUserCloudManage* InManage = UUserCloudManage::Get())
	{
		OutFileLists.Empty();

		TArray<FCloudFileHeader> NewHeaders;
		Ret = InManage->GetUserFileList(NewHeaders);
		for (FCloudFileHeader& Temp : NewHeaders)
		{
			OutFileLists.Add(Temp);
		}
	}
	return Ret;
}

bool UGLCUserCloundLibrary::GetUserFileLists(TArray<FCloudFileHeader>& OutFileLists)
{
	if (UUserCloudManage* InManage = UUserCloudManage::Get())
	{
		return InManage->GetUserFileList(OutFileLists);
	}
	return false;
}
