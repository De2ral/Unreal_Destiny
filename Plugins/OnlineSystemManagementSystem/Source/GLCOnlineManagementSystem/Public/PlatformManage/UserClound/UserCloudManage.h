#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineUserCloudInterface.h"
#include "PlatformManage/Common/OnlineSubsystemManage.h"
#include "Common/Type/GLCCommonType.h"
#include "Interfaces/OnlineUserCloudInterface.h"

#include "UserCloudManage.generated.h"

class UGLCOnlineManageComponent;
class FUniqueNetId;
class UUserCloudManage;
struct FCloudFileHeader;
struct FPlatformManageOptionalParam;
struct FUniqueNetIdRepl;


UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UUserCloudManage : public UOnlineSubsystemManage
{
	GENERATED_UCLASS_BODY()

public:
	static UUserCloudManage* Get();

	//Read file
	virtual bool ReadUserFile(const FString& InFileName,const FPlatformManageOptionalParam& OptionalParam);

	//Get file
	virtual bool GetFileContents(const FString& FileName, TArray<uint8>& FileContents);

	//Write file
	virtual bool WriteUserFile(const FString& FileName,TArray<uint8>& FileContents,
		const FPlatformManageOptionalParam& OptionalParam,bool bCompressBeforeUpload);

	//Delete file
	virtual bool DeleteUserFile(const FString& FileName, const FPlatformManageOptionalParam& OptionalParam,
		bool bShouldCloudDelete, bool bShouldLocallyDelete);

	//Request file form platform
	virtual bool EnumerateUserFiles(const FPlatformManageOptionalParam& OptionalParam);

	//Get user file list
	virtual bool GetUserFileList(TArray<FCloudFileHeader>& UserFiles);

	virtual bool ClearFiles();
	virtual bool ClearFile(const FString& InFileName);


	virtual bool UserCloundManageExe_Internal(const FPlatformManageOptionalParam& OptionalParam,
		TFunction<bool(const FUniqueNetIdRepl& InNetID)> fun,bool bShouldEnqueue,
		FString& InKey);
protected:
	virtual void BeginDestroy()override;

	void ClearAllHandle();

protected:
	IOnlineUserCloudPtr UserCloudPtr;
};
