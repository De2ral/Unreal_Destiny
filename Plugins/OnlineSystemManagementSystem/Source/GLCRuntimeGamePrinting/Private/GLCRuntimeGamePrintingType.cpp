
#include "GLCRuntimeGamePrintingType.h"

namespace GLCRuntimeGamePrinting
{
	FPrintSlot::FSlot& FPrintSlot::FSlot::Color(const FString& InColor)
	{
		ColorContent = InColor;
		return *this;
	}

	FPrintSlot::FSlot& FPrintSlot::FSlot::Image(const FString& InImage)
	{
		Content += FString::Printf(TEXT("<img id=\"%s\">"),*InImage);
		return *this;
	}

	const FString &FPrintSlot::FSlot::ToString() const
	{
		return Content;
	}

	bool FPrintSlot::FSlot::GetString(FString& OutString) const
	{
		OutString = Content;
		return !OutString.IsEmpty();
	}

	FPrintSlot::FSlot& FPrintSlot::AddSlot()
	{
		return Slots.AddDefaulted_GetRef();
	}

	const FString FPrintSlot::ToString() const
	{
		FString OutString;
		GetString(OutString);

		return OutString;
	}

	bool FPrintSlot::GetString(FString& OutString) const
	{
		for (auto& Tmp : Slots)
		{
			OutString.Append(Tmp.ToString());
		}

		return !OutString.IsEmpty();
	}

	FPrintSlot::~FPrintSlot()
	{
		Slots.Empty();
	}
};
