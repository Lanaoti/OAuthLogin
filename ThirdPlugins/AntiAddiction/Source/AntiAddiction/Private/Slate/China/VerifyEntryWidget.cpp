// Fill out your copyright notice in the Description page of Project Settings.


#include "VerifyEntryWidget.h"

#include "AntiAddiction.h"
#include "AntiAddictionUE.h"
#include "GeneralProjectSettings.h"
#include "SlateOptMacros.h"
#include "TapCommon.h"
#include "TapSubsystem.h"
#include "Model/China/AAUChinaConfigModel.h"
#if PLATFORM_DESKTOP
#include "Server/China/Desktop/AAUChinaImpl.h"
#endif
#include "Slate/Common/TapBoxWrapper.h"
#include "Styles/VerifyEntryWidgetStyle.h"
#include "Widgets/Images/SImage.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "TapTap"

void SVerifyEntryWidget::Construct(const FArguments& InArgs, const TSharedRef<AAUImpl>& InImpl, const FAAUChinaAuthIdentifyWord& Word)
{
	Impl = InImpl;
	OnClose = InArgs._OnClose;
	const FVerifyEntryStyle& Style = FModuleManager::GetModuleChecked<FAntiAddictionModule>("AntiAddiction").Style->GetWidgetStyle<FVerifyEntryStyle>("VerifyEntryStyle");

	FText ProjectDisplayName = FText::FromString(FAAUChinaConfigModel::GetLocalModel().IsValid() ? FAAUChinaConfigModel::GetLocalModel()->name : GetDefault<UGeneralProjectSettings>()->ProjectName); 
	ChildSlot
	[
		SAssignNew(Wrapper, STapBoxWrapper, 600.f, 347.f)
			.TitleText(FText::FromString(Word.title))
			.WhiteButtonText(FText::FromString(Word.negative_button))
			.BlueButtonText(FText::FromString(Word.positive_button))
			.OnWhiteButtonClicked(this, &SVerifyEntryWidget::OnManualVerifyButtonClicked)
			.OnBlueButtonClicked(this, &SVerifyEntryWidget::OnTapQuickVerifyButtonClicked)
			.OnCloseButtonClicked(this, &SVerifyEntryWidget::OnCloseButtonClicked)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot() /// Icon
				  .Padding(Style.IconMargin)
				  .AutoHeight()
				  .HAlign(HAlign_Center)
				  .VAlign(VAlign_Fill)
				[
					SNew(SImage)
					.Image(&Style.Icon)
				]
				+ SVerticalBox::Slot() /// Content 0
				  .Padding(Style.ContentMargin)
				  .AutoHeight()
				  .HAlign(HAlign_Fill)
				  .VAlign(VAlign_Fill)
				[
					SNew(STextBlock)
								.TextStyle(&Style.ContentStyle)
								.Justification(ETextJustify::Left)
								.Text(FText::Format(LOCTEXT("QuickVerifyContent0", "《{0}》已接入 TapTap 快速认证服务。"), ProjectDisplayName))
				]
				+ SVerticalBox::Slot() /// Content 1
				  .Padding(Style.ContentMargin)
				  .AutoHeight()
				  .HAlign(HAlign_Fill)
				  .VAlign(VAlign_Fill)
				[
					SNew(STextBlock)
								.TextStyle(&Style.ContentStyle)
								.Justification(ETextJustify::Left)
								.AutoWrapText(true)
								.Text(LOCTEXT("QuickVerifyContent1", "如果你已经在 TapTap 通过了国家网络游戏防沉迷实名认证系统的真实认证，此服务可以帮助你："))
				]
				+ SVerticalBox::Slot() /// Content 2
				  .Padding(Style.ContentMargin)
				  .AutoHeight()
				  .HAlign(HAlign_Fill)
				  .VAlign(VAlign_Fill)
				[
					SNew(STextBlock)
								.TextStyle(&Style.ContentStyle)
								.Justification(ETextJustify::Left)
								.Text(LOCTEXT("QuickVerifyContent2", "快速通过游戏内的实名认证流程"))
				]
				+ SVerticalBox::Slot() /// Content 3
				  .Padding(Style.ContentMargin)
				  .AutoHeight()
				  .HAlign(HAlign_Fill)
				  .VAlign(VAlign_Fill)
				[
					SNew(STextBlock)
								.TextStyle(&Style.ContentStyle)
								.Justification(ETextJustify::Left)
								.Text(LOCTEXT("QuickVerifyContent3", "保护个人敏感信息安全，不泄露给第三方"))
				]
				+ SVerticalBox::Slot() /// Tail
				  .Padding(Style.TailMargin)
				  .AutoHeight()
				  .HAlign(HAlign_Fill)
				  .VAlign(VAlign_Fill)
				[
					SNew(STextBlock)
								.TextStyle(&Style.TailStyle)
								.Justification(ETextJustify::Left)
								.Text(LOCTEXT("QuickVerifyTip", "是否使用快速认证？"))
				]
			]
	];
}

FReply SVerifyEntryWidget::OnManualVerifyButtonClicked()
{
	UTapSubsystem::RemoveWidget(AsShared());
#if PLATFORM_DESKTOP
	if (TSharedPtr<AAUChinaImpl> CImpl = StaticCastSharedPtr<AAUChinaImpl>(Impl))
	{
		CImpl->ShowRealNameUI(AAURealNameWordTypeVerify, false);
	}
#endif
	return FReply::Handled();
}

FReply SVerifyEntryWidget::OnTapQuickVerifyButtonClicked()
{
#if PLATFORM_DESKTOP
	if (TSharedPtr<AAUChinaImpl> CImpl = StaticCastSharedPtr<AAUChinaImpl>(Impl))
	{
		CImpl->GotoTapTapQuickVerify();
	}
#endif
	return FReply::Handled();
}

FReply SVerifyEntryWidget::OnCloseButtonClicked()
{
	UTapSubsystem::RemoveWidget(AsShared());
	OnClose.ExecuteIfBound();
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
