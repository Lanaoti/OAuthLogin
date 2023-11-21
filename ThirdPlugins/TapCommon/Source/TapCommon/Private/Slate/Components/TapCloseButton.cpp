// Fill out your copyright notice in the Description page of Project Settings.


#include "Slate/Components/TapCloseButton.h"

#include "SlateOptMacros.h"
#include "TapCommon.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void STapCloseButton::Construct(const FArguments& InArgs)
{
	TSharedRef<FSlateStyleSet> CommonStyleSet = FModuleManager::GetModuleChecked<FTapCommonModule>("TapCommon").Style.ToSharedRef();
	const FButtonStyle& CloseButtonStyle = CommonStyleSet->GetWidgetStyle<FButtonStyle>("/Button/CloseButtonStyle");
	
	SButton::Construct(SButton::FArguments()
		.OnClicked(InArgs._OnClicked)
		.ButtonStyle(&CloseButtonStyle)
	);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
