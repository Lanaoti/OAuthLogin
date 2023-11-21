// Fill out your copyright notice in the Description page of Project Settings.


#include "Slate/Components/TapWhiteButton.h"

#include "SlateOptMacros.h"
#include "TapCommon.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void STapWhiteButton::Construct(const FArguments& InArgs)
{
	TSharedRef<FSlateStyleSet> CommonStyleSet = FModuleManager::GetModuleChecked<FTapCommonModule>("TapCommon").Style.ToSharedRef();
	const FButtonStyle& WhiteButtonStyle = CommonStyleSet->GetWidgetStyle<FButtonStyle>("/Button/WhiteButtonStyle");
	const FTextBlockStyle& WhiteButtonTextStyle = CommonStyleSet->GetWidgetStyle<FTextBlockStyle>("/Button/WhiteButtonTextStyle");

	SButton::Construct(SButton::FArguments()
		.Text(InArgs._Text)
		.OnClicked(InArgs._OnClicked)
		.ButtonStyle(&WhiteButtonStyle)
		.TextStyle(&WhiteButtonTextStyle)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
	);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
