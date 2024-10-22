

#include "GLCRuntimeGamePrintingLibrary.h"

#include "UObject/ConstructorHelpers.h"
#include "UMG/UI_GamePrintf.h"
#include "Engine/Engine.h"


TSubclassOf<UUI_GamePrintf> UGLCRuntimeGamePrintingLibrary::MyGamePrintClass = nullptr;
	

UGLCRuntimeGamePrintingLibrary::UGLCRuntimeGamePrintingLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUI_GamePrintf> GamePrintfFinder(TEXT("/GLCOnlineManagementSystem/ManagementSystem/UIPrint/UI_GamePrintf_BP"));
	MyGamePrintClass = GamePrintfFinder.Class;
}

APlayerController* UGLCRuntimeGamePrintingLibrary::GetPlayerController(UObject* InObject)
{
	if(GEngine)
	{
		if(UWorld* InWorld = GEngine->GetWorldFromContextObject(InObject,EGetWorldErrorMode::LogAndReturnNull))
		{
			if(APlayerController* InPC = InWorld->GetFirstPlayerController())
			{
				if(InPC->IsLocalController())
				{
					return InPC;
				}
			}
		}
	}
	return nullptr;
}

bool UGLCRuntimeGamePrintingLibrary::RegisterGLCRuntimeGamePrinting(APlayerController* InPlayerController)
{
	if (InPlayerController && InPlayerController->IsLocalController())
	{
		return true;
	}
	return false;
}

bool UGLCRuntimeGamePrintingLibrary::GamePrintLogs(UObject* InObject,
	const TArray<FSimplePrintSlot>& InSlot,
	float InDieDelayTime /*= 7.0f*/)
{
	if (GEngine)
	{
		if (APlayerController* InPlayerController = GetPlayerController(InObject))
		{
			GLCRuntimeGamePrinting::FPrintSlot PrintSlot;
			for (auto& Tmp : InSlot)
			{
				if (!Tmp.ImgID.IsEmpty())
				{
					PrintSlot.AddSlot()
						.Color(Tmp.Color)
						[
							Tmp.Content
						].Image(Tmp.ImgID);
				}
				else
				{
					PrintSlot.AddSlot()
						.Color(Tmp.Color)
						[
							Tmp.Content
						];
				}
			}

			return UGLCRuntimeGamePrintingLibrary::GamePrint_CustomPrintSlot(InObject,PrintSlot,InDieDelayTime);
		}
	}

	return false;
}

bool UGLCRuntimeGamePrintingLibrary::GamePrintLog(UObject* InObject,
	const FSimplePrintSlot& InSlot, float InDieDelayTime /*= 7.0f*/)
{
	TArray<FSimplePrintSlot> Slots;
	Slots.Add(InSlot);

	return GamePrintLogs(InObject,Slots,InDieDelayTime);
}

bool UGLCRuntimeGamePrintingLibrary::GamePrint_CustomPrintSlot(
	UObject* InObject,const GLCRuntimeGamePrinting::FPrintSlot& InSlot,
	float InDieDelayTime /*= 7.0f*/)
{
	return GamePrint_Direct(InObject,FText::FromString(InSlot.ToString()),
		InDieDelayTime);
}

bool UGLCRuntimeGamePrintingLibrary::GamePrint_Direct(
	UObject* InObject,const FText& InText,float InDieDelayTime /*= 7.0f*/)
{
	if (MyGamePrintClass)
	{
		if (APlayerController* InPlayerController = GetPlayerController(InObject))
		{
			if (UUI_GamePrintf* GamePrintfPtr = CreateWidget<UUI_GamePrintf>(InPlayerController, MyGamePrintClass))
			{
				GamePrintfPtr->AddToViewport(200);
				GamePrintfPtr->SetDieDelayTime(InDieDelayTime);

				GamePrintfPtr->SetText(InText);

				GamePrintfPtr->WidgetConstruct();

				return true;
			}
		}
	}

	return false;
}

void UGLCRuntimeGamePrintingLibrary::PrintWhiteLog(UObject* InObject,const FText& InText,float InDieDelayTime /*= 7.f*/)
{
	PrintLogDirectByColor_Internal(InObject,InText, [](FString& Color) {Color = TEXT("white"); }, InDieDelayTime);
}

void UGLCRuntimeGamePrintingLibrary::PrintWhiteLogByFString(UObject* InObject,const FString& InString, 
	float InDieDelayTime /*= 7.f*/)
{
	PrintWhiteLog(InObject,FText::FromString(InString),InDieDelayTime);
}

void UGLCRuntimeGamePrintingLibrary::PrintYellowLog(UObject* InObject,const FText& InText,
	float InDieDelayTime /*= 7.f*/)
{
	PrintLogDirectByColor_Internal(InObject,InText, [](FString& Color) {Color = TEXT("yellow"); }, InDieDelayTime);
}

void UGLCRuntimeGamePrintingLibrary::PrintRedLog(UObject* InObject,const FText& InText,
	float InDieDelayTime /*= 7.f*/)
{
	PrintLogDirectByColor_Internal(InObject,InText,[](FString& Color){Color = TEXT("Red");},InDieDelayTime);
}

void UGLCRuntimeGamePrintingLibrary::PrintRedLogByString(UObject* InObject,
	const FString& InString, float InDieDelayTime /*= 7.f*/)
{
	PrintRedLog(InObject,FText::FromString(InString), InDieDelayTime);
}

void UGLCRuntimeGamePrintingLibrary::PrintRedLogByFString(UObject* InObject,
	const FString& InString, float InDieDelayTime /*= 7.f*/)
{
	PrintRedLog(InObject,FText::FromString(InString),InDieDelayTime);
}

void UGLCRuntimeGamePrintingLibrary::PrintLogDirectByColor_Internal(UObject* InObject,const FText& InText,
	TFunction<void(FString& ColorString)> fun, float InDieDelayTime /*= 7.f*/)
{
	TArray<FSimplePrintSlot> Slots;
	FSimplePrintSlot& NewSlot = Slots.AddDefaulted_GetRef();
	fun(NewSlot.Color);
	NewSlot.Content = InText.ToString();

	GamePrintLogs(InObject,Slots, InDieDelayTime);
}

