// Fill out your copyright notice in the Description page of Project Settings.


#include "Slate/Components/TapBlueButton.h"

#include "SlateOptMacros.h"
#include "TapCommon.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void STapBlueButton::Construct(const FArguments& InArgs)
{
	TSharedRef<FSlateStyleSet> CommonStyleSet = FModuleManager::GetModuleChecked<FTapCommonModule>("TapCommon").Style.ToSharedRef();
	const FButtonStyle& BlueButtonStyle = CommonStyleSet->GetWidgetStyle<FButtonStyle>("/Button/BlueButtonStyle");
	const FTextBlockStyle& BlueButtonTextStyle = CommonStyleSet->GetWidgetStyle<FTextBlockStyle>("/Button/BlueButtonTextStyle");

	SButton::Construct(SButton::FArguments()
		.Text(InArgs._Text)
		.OnClicked(InArgs._OnClicked)
		.ButtonStyle(&BlueButtonStyle)
		.TextStyle(&BlueButtonTextStyle)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
	);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
