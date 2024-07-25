#include "PlatformManage/UserClound/UserCloudManage.h"
#include "OnlineSubsystem.h"
#include "PlatformManage/OnlineSessions/Common/GLCOnlineSessionsLibrary.h"
#include <PlatformManage/Common/GLCPlatformManageLibrary.h>

#include "Engine/GameInstance.h"
#include "Common/Components/GLCOnlineManageComponent.h"
#include "PlatformManage/UserClound/UserCloundType.h"


UUserCloudManage::UUserCloudManage(const FObjectInitializer& Initializer)
   :Super(Initializer)
{
   UserCloudPtr = IOnlineSubsystem::Get()->GetUserCloudInterface();

   if (UserCloudPtr)
   {
      ClearAllHandle();

      UserCloudPtr->AddOnReadUserFileCompleteDelegate_Handle(FOnReadUserFileCompleteDelegate::CreateWeakLambda(this,
         [&](bool bSuccessful, const FUniqueNetId& InNetID, const FString& InFileName)
         {
            if (GetOnlineManageComponent() && GetOnlineManageComponent()->GLCUserCloundObjectClasses.Num())
            {
               RemoveRequest(UserCloundStaticDef::OnReadUserFileCompleteKey,
                  [&](const FPlatformManageOptionalParam& Param)
                  {
                     for (auto& Temp : GetOnlineManageComponent()->GLCUserCloundObjectClasses)
                     {
                        Temp.GetDefaultObject()->OnReadUserFileComplete(bSuccessful, FUniqueNetIdRepl(InNetID),
                           InFileName, Param);
                     }
                  });
            }
         }));

      UserCloudPtr->AddOnWriteUserFileProgressDelegate_Handle(FOnWriteUserFileProgressDelegate::CreateWeakLambda(this,
         [&](int32 CurrentBytes, const FUniqueNetId& InNetID, const FString& InFileName)
         {
            if (GetOnlineManageComponent() && GetOnlineManageComponent()->GLCUserCloundObjectClasses.Num())
            {
               RemoveRequest(UserCloundStaticDef::OnWriteUserFileKey,
                  [&](const FPlatformManageOptionalParam& Param)
                  {
                     for (auto& Temp : GetOnlineManageComponent()->GLCUserCloundObjectClasses)
                     {
                        Temp.GetDefaultObject()->OnWriteUserFileInProgress(CurrentBytes, FUniqueNetIdRepl(InNetID),
                           InFileName, Param);
                     }
                  });
            }
         }));

      UserCloudPtr->AddOnWriteUserFileCompleteDelegate_Handle(FOnWriteUserFileCompleteDelegate::CreateWeakLambda(this,
         [&](bool bSuccessful, const FUniqueNetId& InNetID, const FString& InFileName)
         {
            if (GetOnlineManageComponent() && GetOnlineManageComponent()->GLCUserCloundObjectClasses.Num())
            {
               RemoveRequest(UserCloundStaticDef::OnWriteUserFileKey,
                  [&](const FPlatformManageOptionalParam& Param)
                  {
                     for (auto& Temp : GetOnlineManageComponent()->GLCUserCloundObjectClasses)
                     {
                        Temp.GetDefaultObject()->OnWriteUserFileComplete(bSuccessful, FUniqueNetIdRepl(InNetID), InFileName,
                           Param);
                     }
                  });
            }
         }));

      UserCloudPtr->AddOnDeleteUserFileCompleteDelegate_Handle(FOnDeleteUserFileCompleteDelegate::CreateWeakLambda(this,
         [&](bool bSuccessful, const FUniqueNetId& InNetID, const FString& InFileName)
         {
            if (GetOnlineManageComponent() && GetOnlineManageComponent()->GLCUserCloundObjectClasses.Num())
            {
               RemoveRequest(UserCloundStaticDef::OnDeleteUserFileCompleteKey,
                  [&](const FPlatformManageOptionalParam& Param)
                  {
                     for (auto& Temp : GetOnlineManageComponent()->GLCUserCloundObjectClasses)
                     {
                        Temp.GetDefaultObject()->OnDeleteUserFileComplete(bSuccessful, FUniqueNetIdRepl(InNetID),
                           InFileName, Param);
                     }
                  });
            }
         }));

      UserCloudPtr->AddOnEnumerateUserFilesCompleteDelegate_Handle(FOnEnumerateUserFilesCompleteDelegate::CreateWeakLambda(this,
         [&](bool bSuccessful, const FUniqueNetId& InNetID)
         {
            if (GetOnlineManageComponent() && GetOnlineManageComponent()->GLCUserCloundObjectClasses.Num())
            {
               RemoveRequest(UserCloundStaticDef::OnEnumerateUserFileCompleteKey,
                  [&](const FPlatformManageOptionalParam& Param)
                  {
                     for (auto& Temp : GetOnlineManageComponent()->GLCUserCloundObjectClasses)
                     {
                        Temp.GetDefaultObject()->OnEnumerateUserFileComplete(bSuccessful,
                           FUniqueNetIdRepl(InNetID), Param);
                     }
                  });
            }
         }));
   }
}

void UUserCloudManage::BeginDestroy()
{
   ClearAllHandle();

   Super::BeginDestroy();
}

void UUserCloudManage::ClearAllHandle()
{
   if (UserCloudPtr)
   {
      UserCloudPtr->ClearOnReadUserFileCompleteDelegates(this);

      UserCloudPtr->ClearOnWriteUserFileCompleteDelegates(this);

      UserCloudPtr->ClearOnWriteUserFileCompleteDelegates(this);

      UserCloudPtr->ClearOnDeleteUserFileCompleteDelegates(this);

      UserCloudPtr->ClearOnEnumerateUserFilesCompleteDelegates(this);
   }
}

UUserCloudManage* UUserCloudManage::Get()
{
   if(UGameInstance* InGI = UGLCPlatformManageLibrary::GetGameInstance())
   {
      return InGI->GetSubsystem<UUserCloudManage>();
   }
   return nullptr;
}

bool UUserCloudManage::ReadUserFile(const FString& InFileName, const FPlatformManageOptionalParam& OptionalParam)
{
   FString InKey = UserCloundStaticDef::OnReadUserFileCompleteKey;
   return UserCloundManageExe_Internal(OptionalParam, [&](const FUniqueNetIdRepl& InNetID)->bool
      {
         return UserCloudPtr->ReadUserFile(*InNetID, InFileName);
      }, true, InKey);
}

bool UUserCloudManage::GetFileContents(const FString& FileName,
   TArray<uint8>& FileContents)
{
   FString InKey;
   return UserCloundManageExe_Internal(FPlatformManageOptionalParam(), [&](const FUniqueNetIdRepl& InNetID)->bool
      {
         return UserCloudPtr->GetFileContents(*InNetID, FileName, FileContents);
      }, false, InKey);
}

bool UUserCloudManage::WriteUserFile(const FString& FileName, TArray<uint8>& FileContents, const FPlatformManageOptionalParam& OptionalParam, bool bCompressBeforeUpload /*= false*/)
{
   FString InKey = UserCloundStaticDef::OnWriteUserFileKey;
   return UserCloundManageExe_Internal(OptionalParam, [&](const FUniqueNetIdRepl& InNetID)->bool
      {
         return UserCloudPtr->WriteUserFile(*InNetID, FileName, FileContents, bCompressBeforeUpload);
      }, true, InKey);
}

bool UUserCloudManage::DeleteUserFile(const FString& FileName,
   const FPlatformManageOptionalParam& OptionalParam, bool bShouldCloudDelete, bool bShouldLocallyDelete)
{
   FString InKey = UserCloundStaticDef::OnDeleteUserFileCompleteKey;
   return UserCloundManageExe_Internal(OptionalParam, [&](const FUniqueNetIdRepl& InNetID)->bool
      {
         return UserCloudPtr->DeleteUserFile(*InNetID, FileName, bShouldCloudDelete, bShouldLocallyDelete);
      }, true, InKey);
}

bool UUserCloudManage::EnumerateUserFiles(const FPlatformManageOptionalParam& OptionalParam)
{
   FString InKey = UserCloundStaticDef::OnEnumerateUserFileCompleteKey;
   return UserCloundManageExe_Internal(OptionalParam, [&](const FUniqueNetIdRepl& InNetID)->bool
      {
         UserCloudPtr->EnumerateUserFiles(*InNetID);
         return true;
      }, true, InKey);
}

bool UUserCloudManage::GetUserFileList(TArray<FCloudFileHeader>& UserFiles)
{
   FString InKey;
   return UserCloundManageExe_Internal(FPlatformManageOptionalParam(), [&](const FUniqueNetIdRepl& InNetID)->bool
      {
         UserCloudPtr->GetUserFileList(*InNetID, UserFiles);
         return true;
      }, false, InKey);
}

bool UUserCloudManage::ClearFiles()
{
   FString InKey;
   return UserCloundManageExe_Internal(FPlatformManageOptionalParam(), [&](const FUniqueNetIdRepl& InNetID)->bool
      {
         UserCloudPtr->ClearFiles(*InNetID);
         return true;
      }, false, InKey);
}

bool UUserCloudManage::ClearFile(const FString& InFileName)
{
   FString InKey;
   return UserCloundManageExe_Internal(FPlatformManageOptionalParam(), [&](const FUniqueNetIdRepl& InNetID)->bool
      {
         UserCloudPtr->ClearFile(*InNetID, InFileName);
         return true;
      }, false, InKey);
}

bool UUserCloudManage::UserCloundManageExe_Internal(const FPlatformManageOptionalParam& OptionalParam,
   TFunction<bool(const FUniqueNetIdRepl& InNetID)> fun, bool bShouldPublish, FString& InKey)
{
   if (UserCloudPtr)
   {
      FUniqueNetIdRepl NetID;
      if (UGLCPlatformManageLibrary::GetUniqueNetIDFromObject(GetOnlineManageComponent(), NetID))
      {
         if (bShouldPublish)
         {
            if (!PublishRequest(OptionalParam, InKey))
            {
               return false;
            }
         }
         return fun(NetID);
      }
   }
   return false;
}


