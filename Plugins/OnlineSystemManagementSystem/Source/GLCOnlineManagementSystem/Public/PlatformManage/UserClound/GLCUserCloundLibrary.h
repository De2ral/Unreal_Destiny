#pragma once

#include "OnlineSubsystemTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UserCloundType.h"
#include "Common/Type/GLCCommonType.h"

#include "GLCUserCloundLibrary.generated.h"

class USaveGame;
struct FPlatformManageOptionalParam;
struct FCloudFileHeader;


/** Holds metadata about a given downloadable file */
USTRUCT(BlueprintType)
struct GLCONLINEMANAGEMENTSYSTEM_API FGLCCloudFileHeader
{
	GENERATED_USTRUCT_BODY()

		/** Hash value, if applicable, of the given file contents */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CloudFileHeader")
		FString Hash;
	/** The hash algorithm used to sign this file */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CloudFileHeader")
		FName HashType;
	/** Filename as downloaded */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CloudFileHeader")
		FString DLName;
	/** Logical filename, maps to the downloaded filename */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CloudFileHeader")
		FString FileName;
	/** File size */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CloudFileHeader")
		int32 FileSize;
	/** The full URL to download the file if it is stored in a CDN or separate host site */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CloudFileHeader")
		FString URL;
	/** The chunk id this file represents */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CloudFileHeader")
		int32 ChunkID;
	/** Pointers to externally-accessible representations of this file */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CloudFileHeader")
		TMap<FString, FString> ExternalStorageIds;

	/** Constructors */
	FGLCCloudFileHeader() :
		FileSize(0),
		ChunkID(0)
	{}
	FGLCCloudFileHeader(const FCloudFileHeader& InHeader)
	{
		ConstructGLCCloudFileHeader(InHeader);
	}

	FGLCCloudFileHeader& operator=(const FCloudFileHeader& InHeader)
	{
		ConstructGLCCloudFileHeader(InHeader);
		return *this;
	}
	void ConstructGLCCloudFileHeader(const FCloudFileHeader& InHeader)
	{
		this->ChunkID = InHeader.ChunkID;
		this->DLName = InHeader.DLName;
		this->ExternalStorageIds = InHeader.ExternalStorageIds;
		this->FileName = InHeader.FileName;
		this->FileSize = InHeader.FileSize;
		this->Hash = InHeader.Hash;
		this->HashType = InHeader.HashType;
		this->URL = InHeader.URL;
	}

};


UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UGLCUserCloundLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	//Send read file network request
	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|UserClound", meta = (DisplayName = "ReadUserFile"))
		static bool K2_ReadUserFile(const FString& InFileName
			, FPlatformManageOptionalParam OptionalParam);

	static bool ReadUserFile(const FString& InFileName
		, const FPlatformManageOptionalParam& OptionalParam);

	//Get file contents
	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|UserClound")
		static bool GetFileContentsAsMemory(const FString& FileName, TArray<uint8>& FileContents);

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|UserClound")
		static bool GetFileContentsAsString(const FString& FileName, FString& OutString, bool bMD5Decryption = true);

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|UserClound")
		static USaveGame* GetFileContentsAsSaveGame(const FString& FileName, bool bMD5Decryption = true);

	//Write file
	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|UserClound", meta = (DisplayName = "WriteUserFileFromMemory"))
		static bool K2_WriteUserFileFromMemory(const FString& FileName, const TArray<uint8>& FileContents,
			FPlatformManageOptionalParam OptionalParam,
			bool bCompressBeforeUpload = false);

	//Write file
	static bool WriteUserFileFromMemory(const FString& FileName, TArray<uint8>& FileContents,
		const FPlatformManageOptionalParam& OptionalParam = FPlatformManageOptionalParam(),
		bool bCompressBeforeUpload = false);

	//Write file
	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|UserClound", meta = (DisplayName = "WriteUserFileFromString"))
		static bool K2_WriteUserFileFromString(const FString& FileName, const FString& FileContents,
			FPlatformManageOptionalParam OptionalParam,
			bool bCompressBeforeUpload = false, bool bMD5Encrypt = true);

	static bool WriteUserFileFromString(const FString& FileName, const FString& FileContents,
		const FPlatformManageOptionalParam& OptionalParam,
		bool bCompressBeforeUpload = false, bool bMD5Encrypt = true);

	//Write file
	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|UserClound", meta = (DisplayName = "WriteUserFileFromSaveGame"))
		static bool K2_WriteUserFileFromSaveGame(const FString& FileName, USaveGame* InSaveGame,
			FPlatformManageOptionalParam OptionalParam, bool bCompressBeforeUpload = false, bool bMD5Encrypt = true);

	static bool WriteUserFileFromSaveGame(const FString& FileName, USaveGame* InSaveGame,
		const FPlatformManageOptionalParam& OptionalParam,
		bool bCompressBeforeUpload = false, bool bMD5Encrypt = true);

	//Delete file
	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|UserClound", meta = (DisplayName = "DeleteUserFile"))
		static bool K2_DeleteUserFile(const FString& FileName, FPlatformManageOptionalParam OptionalParam,
			bool bShouldCloudDelete = true, bool bShouldLocallyDelete = true);

	static bool DeleteUserFile(const FString& FileName, const FPlatformManageOptionalParam& OptionalParam,
		bool bShouldCloudDelete = true, bool bShouldLocallyDelete = true);

	//Enumerate files
	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|UserClound", meta = (DisplayName = "EnumerateUserFiles"))
		static bool K2_EnumerateUserFiles(FPlatformManageOptionalParam OptionalParam);

	static bool EnumerateUserFiles(const FPlatformManageOptionalParam& OptionalParam);

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|UserClound", meta = (DisplayName = "ClearFiles"))
		static bool ClearFiles();

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|UserClound", meta = (DisplayName = "ClearFile"))
		static bool ClearFile(const FString& InFileName);

	//Get user file list
	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|UserClound", meta = (DisplayName = "GetUserFileLists"))
		static bool K2_GetUserFileLists(TArray<FGLCCloudFileHeader>& OutFileLists);

	//Get user file list
	static bool GetUserFileLists(TArray<FCloudFileHeader>& OutFileLists);

};
