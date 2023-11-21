﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Slate/Widgets/TapMessageBox.h"

#include "SlateOptMacros.h"
#include "TapCommon.h"
#include "TapSubsystem.h"
#include "Slate/Common/TapBoxWrapper.h"
#include "Slate/Styles/TapMessageBoxWidgetStyle.h"
#include "Widgets/Text/SRichTextBlock.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
#define LOCTEXT_NAMESPACE "TapTap"

void STapMessageBox::Construct(const FArguments& InArgs)
{
	const FTapMessageBoxStyle& Style = FModuleManager::GetModuleChecked<FTapCommonModule>("TapCommon").Style->GetWidgetStyle<FTapMessageBoxStyle>("TapMessageBoxStyle");

	FString Content = InArgs._ContentRichText.ToString();
	Content.ReplaceInline(TEXT("<span color=\"#888888\">"), TEXT(""));
	Content.ReplaceInline(TEXT("</span>"), TEXT(""));
	Content.ReplaceInline(TEXT("<b>"), TEXT(""));
	Content.ReplaceInline(TEXT("</b>"), TEXT(""));
	Content.ReplaceInline(TEXT("<br>"), TEXT("\n"));
	Content.ReplaceInline(TEXT("<font color=\"#FF8156\">"), TEXT("<Orange>"));
	Content.ReplaceInline(TEXT("</font>"), TEXT("</>"));

	bCloseBoxAfterWhiteButtonClicked = InArgs._bCloseBoxAfterWhiteButtonClicked;
	bCloseBoxAfterBlueButtonClicked = InArgs._bCloseBoxAfterBlueButtonClicked;
	WhiteButtonClicked = InArgs._OnWhiteButtonClicked;
	BlueButtonClicked = InArgs._OnBlueButtonClicked;
	
	ChildSlot
	[
		SAssignNew(Wrapper, STapBoxWrapper, 600.f, 347.f)
		.VAlign(VAlign_Center)
		.Padding(Style.ContentPadding)
		.TitleText(InArgs._TitleText)
		.BlueButtonText(InArgs._BlueButtonText)
		.WhiteButtonText(InArgs._WhiteButtonText)
		.bShowCloseButton(InArgs._bShowCloseButton)
		.OnBlueButtonClicked(this, &STapMessageBox::OnBlueButtonClicked)
		.OnWhiteButtonClicked(this, &STapMessageBox::OnWhiteButtonClicked)
		.OnCloseButtonClicked(this, &STapMessageBox::OnCloseButtonClicked)
		[
			InArgs._Content.Widget == SNullWidget::NullWidget
			? SNew(SRichTextBlock)
				.Text(FText::FromString(Content))
				.TextStyle(&Style.RichTextBlockStyle)
				.DecoratorStyleSet(FModuleManager::GetModuleChecked<FTapCommonModule>("TapCommon").Style.Get())
				.Justification(ETextJustify::Center)
				.AutoWrapText(true)
			: InArgs._Content.Widget
		]
	];
}

FReply STapMessageBox::OnWhiteButtonClicked()
{
	WhiteButtonClicked.ExecuteIfBound(StaticCastSharedRef<STapMessageBox>(AsShared()));
	if (bCloseBoxAfterWhiteButtonClicked)
	{
		UTapSubsystem::RemoveWidget(AsShared());
	}
	return FReply::Handled();
}

FReply STapMessageBox::OnBlueButtonClicked()
{
	BlueButtonClicked.ExecuteIfBound(StaticCastSharedRef<STapMessageBox>(AsShared()));
	if (bCloseBoxAfterBlueButtonClicked)
	{
		UTapSubsystem::RemoveWidget(AsShared());
	}
	return FReply::Handled();
}

FReply STapMessageBox::OnCloseButtonClicked()
{
	UTapSubsystem::RemoveWidget(AsShared());
	CloseButtonClicked.ExecuteIfBound();
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
