#pragma once
#include "CoreMinimal.h"
#include "GLCRuntimeGamePrintingType.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FSimplePrintSlot
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimplePrintSlot")
	FString Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimplePrintSlot")
	FString Content;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimplePrintSlot")
	FString ImgID;
};

namespace GLCRuntimeGamePrinting
{
	struct GLCRUNTIMEGAMEPRINTING_API FPrintSlot
	{
		struct GLCRUNTIMEGAMEPRINTING_API FSlot
		{
			FSlot& Color(const FString& InColor);
			FSlot& Image(const FString& InImage);

			FSlot& operator[](const FString& InContent)
			{
				if (!ColorContent.IsEmpty())
				{
					Content += FString::Printf(TEXT("<%s>%s</>"), *ColorContent, *InContent);
				}

				return *this;
			}

			const FString& ToString() const;
			bool GetString(FString& OutString) const;

		protected:
			FString Content;
			FString ColorContent;
		};

		FSlot& AddSlot();

		const FString ToString() const;
		bool GetString(FString& OutString) const;

		~FPrintSlot();

	protected:
		TArray<FSlot> Slots;
	};
}
