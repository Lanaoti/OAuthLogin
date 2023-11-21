// Fill out your copyright notice in the Description page of Project Settings.


#include "ManualVerifyWidget.h"

#include "AntiAddiction.h"
#include "AntiAddictionUE.h"
#include "SlateOptMacros.h"
#include "TapCommon.h"
#include "TapSubsystem.h"
#include "TUDataStorage.h"
#include "Server/AAUHelper.h"
#include "Server/AAUImpl.h"
#include "Server/AAUStorage.h"
#if PLATFORM_DESKTOP
#include "Server/China/Desktop/AAUChinaImpl.h"
#include "Server/China/Desktop/AAUChinaRealName.h"
#endif
#include "Slate/Common/TapBoxWrapper.h"
#include "Slate/Widgets/TapToast.h"
#include "Styles/ManualVerifyWidgetStyle.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SScrollBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
#define LOCTEXT_NAMESPACE "TapTap"

void SManualVerifyWidget::Construct(const FArguments& InArgs, const TSharedRef<AAUImpl>& InImpl, const FAAUChinaAuthIdentifyWord& Word, bool bInFallback)
{
	Impl = InImpl;
	bFallback = bInFallback;
	OnClose = InArgs._OnClose;
	const FManualVerifyStyle& Style = FModuleManager::GetModuleChecked<FAntiAddictionModule>("AntiAddiction").Style->GetWidgetStyle<FManualVerifyStyle>("ManualVerifyStyle");

	ChildSlot
	[
		SNew(STapBoxWrapper, 600.f, 347.f)
		.TitleText(FText::FromString(Word.title))
		.BlueButtonText(FText::FromString(Word.positive_button))
		.OnBlueButtonClicked(this, &SManualVerifyWidget::OnCommitButtonClicked)
		.OnCloseButtonClicked(this, &SManualVerifyWidget::OnCloseButtonClicked)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot() /// Content
			  .Padding(Style.ContentMargin)
			  .FillHeight(1.f)
			  .HAlign(HAlign_Fill)
			  .VAlign(VAlign_Fill)
			[
				SNew(SScrollBox)
				.Style(&Style.ContentScrollBoxStyle)
				.Orientation(Orient_Vertical)
				.ScrollBarVisibility(EVisibility::Collapsed)
				.AllowOverscroll(EAllowOverscroll::No)
				+ SScrollBox::Slot()
				[
					SNew(STextBlock)
							.TextStyle(&Style.ContentStyle)
							.Justification(ETextJustify::Left)
							.AutoWrapText(true)
							.Text(FText::FromString(AAUHelper::ReplaceHtmlTag(Word.description)))
				]
			]
			+ SVerticalBox::Slot() /// Content Name
			  .Padding(Style.ContentMargin)
			  .AutoHeight()
			  .HAlign(HAlign_Center)
			  .VAlign(VAlign_Fill)
			[
				SNew(SBox)
				.HeightOverride(38.f)
				.WidthOverride(294.f)
				[
					SAssignNew(NameBox, SEditableTextBox)
							.Style(&Style.InputBoxStyle)
							.Justification(ETextJustify::Left)
							.HintText(LOCTEXT("VerifyRealName", "真实姓名"))
				]
			]
			+ SVerticalBox::Slot() /// Content Card id
			  .Padding(Style.ContentMargin)
			  .AutoHeight()
			  .HAlign(HAlign_Center)
			  .VAlign(VAlign_Fill)
			[
				SNew(SBox)
				.HeightOverride(38.f)
				.WidthOverride(294.f)
				[
					SAssignNew(CardIdBox, SEditableTextBox)
							.Style(&Style.InputBoxStyle)
							.Justification(ETextJustify::Left)
							.HintText(LOCTEXT("VerifyCardId", "请输入正确的身份证号码"))
				]
			]
		]
	];

	FSlateApplication::Get().SetKeyboardFocus(NameBox.ToSharedRef());
}

FReply SManualVerifyWidget::OnCommitButtonClicked()
{
	if (!Impl)
	{
		return FReply::Handled();
	}
#if PLATFORM_DESKTOP
	TSharedRef<AAUChinaImpl> CImpl = StaticCastSharedRef<AAUChinaImpl>(Impl.ToSharedRef());
	FString Name = NameBox->GetText().ToString();
	FString CardId = CardIdBox->GetText().ToString();
	FString UserId = CImpl->CurrentUserID;
	if (Name.IsEmpty())
	{
		STapToast::Show(ETapToastType::Error, LOCTEXT("Name is empty", "姓名不能为空"));
		FSlateApplication::Get().SetKeyboardFocus(NameBox);
	}
	else if (CardId.IsEmpty())
	{
		STapToast::Show(ETapToastType::Error, LOCTEXT("Card id is empty", "身份信息不能为空"));
		FSlateApplication::Get().SetKeyboardFocus(CardIdBox);
	}
	else
	{
		FTapCommonModule::TapThrobberShowWait();
		AAUChinaRealName::ManualVerify(UserId, Name, CardId, bFallback, [CImpl, this, Name, CardId, UserId](TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)
		{
			FTapCommonModule::TapThrobberDismiss();
			if (ModelPtr.IsValid())
			{
				switch (ModelPtr->GetAuthState())
				{
				case FAAURealNameResultModel::Success:
					{
						// 判断是否是单机校验的身份证，如果是，缓存身份证给下次登录校验
						if (ModelPtr->anti_addiction_token == AAUImpl::LocalTokenString)
						{
							TUDataStorage<FAAUStorage>::SaveString(FAAUStorage::SubmitCardName + UserId, Name);
							TUDataStorage<FAAUStorage>::SaveString(FAAUStorage::SubmitCardID + UserId, CardId);
						}
						CImpl->Login(ModelPtr->anti_addiction_token, AAUHelper::MakeAgeLimit(ModelPtr->age_limit), true);
						STapToast::Show(ETapToastType::Success, LOCTEXT("CommitSuccess", "提交成功"));
						UTapSubsystem::RemoveWidget(AsShared());
					}
					break;
				case FAAURealNameResultModel::Verifying:
					CImpl->ShowRealNameUI(AAURealNameWordTypeVerifying);
					break;
				case FAAURealNameResultModel::Fail:
					CImpl->ShowRealNameUI(AAURealNameWordTypeVerifyFail);
					break;
				}
			}
			else
			{
				STapToast::Show(ETapToastType::Error, FText::FromString(Error.GetErrorReason()));
			}
		});
	}
#endif
	return FReply::Handled();
}

FReply SManualVerifyWidget::OnCloseButtonClicked()
{
	UTapSubsystem::RemoveWidget(AsShared());
	OnClose.ExecuteIfBound();
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
