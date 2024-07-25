// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameFramework/OnlineReplStructs.h"

#include "GLCVoiceLibrary.generated.h"

struct FUniqueNetIdRepl;
class UUI_VoiceChatMain;
class UUI_VoiceChatInfo;


UCLASS()
class GLCONLINEMANAGEMENTSYSTEM_API UGLCVoiceLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UGLCVoiceLibrary();
	static TSubclassOf<UUI_VoiceChatMain> VoiceChatMainClass;
	static TSubclassOf<UUI_VoiceChatInfo> VoiceChatInfoClass;

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|GLCVoiceLibrary", meta = (DisplayName = "Start Networked Voice"))
		static void StartNetworkedVoice();
	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|GLCVoiceLibrary", meta = (DisplayName = "Stop Networked Voice"))
		static void StopNetworkedVoice();

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|GLCVoiceLibrary", meta = (DisplayName = "Mute Player"))
		static void MutePlayer(const FUniqueNetIdRepl& PlayerNetId);
	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|GLCVoiceLibrary", meta = (DisplayName = "Unmute Player"))
		static void UnmutePlayer(const FUniqueNetIdRepl& PlayerNetId);
	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|GLCVoiceLibrary", meta = (DisplayName = "Set Is Mute Player"))
		static void SetIsMutePlayer(const FUniqueNetIdRepl& PlayerNetId, bool bIsMute);

	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|GLCVoiceLibrary", meta = (DisplayName = "Is Player Muted"))
		static bool IsPlayerMuted(const FUniqueNetIdRepl& PlayerId);

	static UUI_VoiceChatMain* CreateVoiceChatWidget(bool bInputUIOnly = true, bool bShowMouse = true);
	UFUNCTION(BlueprintCallable, Category = "GLCPlatformManage|GLCVoiceLibrary", meta = (DisplayName = "Show Voice Chat Widget"))
	static void ShowVoiceChatWidget();

};
