#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "GLCRuntimeGamePrintingType.h"
#include "UMG/UI_GamePrintf.h"

#include "GLCRuntimeGamePrintingLibrary.generated.h"



UCLASS()
class GLCRUNTIMEGAMEPRINTING_API UGLCRuntimeGamePrintingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	static TSubclassOf<UUI_GamePrintf> MyGamePrintClass;
	static APlayerController* GetPlayerController(UObject* InObject);

	//This function had been discorded
	UFUNCTION(BlueprintCallable, Category = "GLCRuntimeGamePrintingLibrary")
	static bool RegisterGLCRuntimeGamePrinting(APlayerController* InPlayerController);

	UFUNCTION(BlueprintCallable,Category="GLCRuntimeGamePrintingLibrary")
	static bool GamePrintLogs(UObject* InObject,
		const TArray<FSimplePrintSlot>& InSlot,
		float InDieDelayTime = 7.0f);

	UFUNCTION(BlueprintCallable, Category = "GLCRuntimeGamePrintingLibrary")
	static bool GamePrintLog(
		UObject* InObject,const FSimplePrintSlot& InSlot,
		float InDieDelayTime = 7.0f);
	
	UFUNCTION(BlueprintCallable,Category="GLCRuntimeGamePrintingLibrary")
	static void PrintWhiteLog(UObject* InObject,const FText& InText, float InDieDelayTime = 7.f);

	UFUNCTION(BlueprintCallable, Category = "GLCRuntimeGamePrintingLibrary")
	static void PrintWhiteLogByFString(UObject* InObject,const FString& InString, float InDieDelayTime = 7.f);
	
	UFUNCTION(BlueprintCallable,Category="GLCRuntimeGamePrintingLibrary")
	static void PrintYellowLog(UObject* InObject,const FText& InText, float InDieDelayTime = 7.f);
	
	UFUNCTION(BlueprintCallable,Category="GLCRuntimeGamePrintingLibrary")
	static void PrintRedLog(UObject* InObject,const FText& InText, float InDieDelayTime = 7.f);

	UFUNCTION(BlueprintCallable, Category = "GLCRuntimeGamePrintingLibrary")
	static void PrintRedLogByString(UObject* InObject,const FString& InString, float InDieDelayTime = 7.f);

	UFUNCTION(BlueprintCallable, Category = "GLCRuntimeGamePrintingLibrary")
	static void PrintRedLogByFString(UObject* InObject,const FString& InString, float InDieDelayTime = 7.f);

	static void PrintLogDirectByColor_Internal(UObject* InObject,const FText& InText,
		TFunction<void(FString& ColorString)> fun, float InDieDelayTime = 7.f);

	static bool GamePrint_CustomPrintSlot(
		UObject* InObject,const GLCRuntimeGamePrinting::FPrintSlot& InSlot,
		float InDieDelayTime = 7.0f);

	UFUNCTION(BlueprintCallable, Category = "GLCRuntimeGamePrintingLibrary")
	static bool GamePrint_Direct(
		UObject* InObject,const FText& InText,
		float InDieDelayTime = 7.0f);
	
};
