#include "AAUChinaImpl.h"

#include "AAUChinaRealName.h"
#include "AntiAddictionUE.h"
#include "TapCommon.h"
#include "TapSubsystem.h"
#include "TUDebuger.h"
#include "TUSettings.h"
#include "Model/AAUUser.h"
#include "Server/AAUHelper.h"
#include "Server/AAUNet.h"
#include "Server/AAUStorage.h"
#include "Slate/China/ManualVerifyWidget.h"
#include "Slate/Widgets/TapMessageBox.h"
#include "Slate/Widgets/TapToast.h"
#include "Slate/China/VerifyEntryWidget.h"
#include "TapUELogin.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Desktop/Slate/TapLoginWidget.h"
#include "Model/China/TapSmoothError.h"

#define LOCTEXT_NAMESPACE "TapTap"

AAUChinaImpl::AAUChinaImpl()
{
	Server = MakeShareable(new AAUChinaSever);
	Server->Delegate = this;
}

AAUChinaImpl::~AAUChinaImpl()
{
}

void AAUChinaImpl::InitImpl(const FAAUConfig& _Config)
{
	AAUNet::GetSDKConfig([](TSharedPtr<FAAUChinaConfigModel> ModelPtr, const FTUError& Error)
	{
		if (ModelPtr.IsValid())
		{
			FAAUChinaConfigModel::SaveToLocal(ModelPtr);
		}
		else
		{
			TUDebuger::WarningLog(FString::Printf(TEXT("Request AA Config Fail: %s"), *Error.GetErrorReason()));
		}
	});
}

void AAUChinaImpl::StartupWithTapTap(const FString& UserId)
{
	CurrentUserID = UserId;
	if (TSharedPtr<SManualVerifyWidget> Widget = ManualVerifyWidget.Pin())
	{
		UTapSubsystem::RemoveWidget(Widget.ToSharedRef());
	}
	if (TSharedPtr<SVerifyEntryWidget> Widget = EntryWidget.Pin())
	{
		UTapSubsystem::RemoveWidget(Widget.ToSharedRef());
	}

	TSharedPtr<FAAUUser> LoginUser = TUDataStorage<FAAUStorage>::LoadStruct<FAAUUser>(FAAUStorage::HasLoginedUser + UserId);
	FTapCommonModule::TapThrobberShowWait();
	auto ResultBlock = [=](TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)
	{
		FTapCommonModule::TapThrobberDismiss();
		if (ModelPtr.IsValid())
		{
			switch (ModelPtr->GetAuthState())
			{
			case FAAURealNameResultModel::Success:
				Login(ModelPtr->anti_addiction_token, AAUHelper::MakeAgeLimit(ModelPtr->age_limit));
				break;
			case FAAURealNameResultModel::Verifying:
				ShowRealNameUI(AAURealNameWordTypeVerifying);
				break;
			case FAAURealNameResultModel::Fail:
				if (ModelPtr->has_auth_record)
				{
					ShowRealNameUI(AAURealNameWordTypeVerifyFail);
				}
				if (TapUELogin::GetAccessToken().IsValid())
				{
					GotoTapTapQuickVerify();
				}
				else
				{
					ShowRealNameUI(AAURealNameWordTypeQuickVerify);
				}
				break;
			}
		}
		else
		{
			IsNeedStandAlone([=](bool IsNeed)
			{
				if (LoginUser.IsValid() && !LoginUser->AccessToken.IsEmpty())
				{
					if (LoginUser->AccessToken == LocalTokenString && !IsNeed)
					{
						TryAgainLogin(Error.GetErrorReason());
					}
					else
					{
						Login(LoginUser->AccessToken, LoginUser->AgeLimit);
					}
				}
				else
				{
					if (IsNeed)
					{
						ShowRealNameUI(AAURealNameWordTypeVerify);
					}
					else
					{
						TryAgainLogin(Error.GetErrorReason());
					}
				}
			});
		}
	};

	if (LoginUser.IsValid() && LoginUser->AccessToken == LocalTokenString)
	{
		FString SubmitCardName = TUDataStorage<FAAUStorage>::LoadString(FAAUStorage::SubmitCardName + UserId);
		FString SubmitCardID = TUDataStorage<FAAUStorage>::LoadString(FAAUStorage::SubmitCardID + UserId);
		if (!SubmitCardName.IsEmpty() && !SubmitCardID.IsEmpty())
		{
			AAUChinaRealName::ManualVerify(UserId, SubmitCardName, SubmitCardID, true, ResultBlock);
			return;
		}
	}
	AAUChinaRealName::CheckRealNameState(UserId, ResultBlock);
}

void AAUChinaImpl::SetTestEnv(bool Enable)
{
	bTestEnvEnable = Enable;
	static UUserWidget* widget = nullptr;
	if (Enable && widget == nullptr)
	{
		if (UClass* MyWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/AntiAddiction/AAUEnvTipWidget.AAUEnvTipWidget_C'")))
		{
			if (TUSettings::GetGameInstance().IsValid())
			{
				widget = CreateWidget<UUserWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
				widget->AddToViewport(INT32_MAX - 10);
			}
		}
	}
	else
	{
		if (widget != nullptr)
		{
			widget->RemoveFromParent();
			widget = nullptr;
		}
	}
}

void AAUChinaImpl::Exit()
{
	AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::Exited, "");
	Server->Logout();
}

EAAUAgeLimit AAUChinaImpl::GetAgeRange()
{
	return Server->GetAgeRange();
}

int AAUChinaImpl::GetRemainingTime()
{
	return Server->GetRemainingTime();
}

void AAUChinaImpl::KickOut(AAUTimeBoundary Boundary, const FString& Title, const FString& Content)
{
	if (Boundary == AAUTimeBoundaryCurfew)
	{
		ShowHealthTipUI(Title, Content, 0, AAUHealthTipTypeInCurfew);
	}
	else if (Boundary == AAUTimeBoundaryTimeLimit)
	{
		ShowHealthTipUI(Title, Content, 0, AAUHealthTipTypeTimeout);
	}
	else
	{
		Exit();
	}
}

void AAUChinaImpl::EnterGame()
{
	Server->EnterGame();
}

void AAUChinaImpl::LeaveGame()
{
	Server->LeaveGame();
}

void AAUChinaImpl::CheckPayLimit(int Amount, TFunction<void(bool Status)> CallBack,
                                 TFunction<void(const FString& Msg)> FailureHandler)
{
	Server->CheckPayLimit(Amount, [=](bool Status, const FString& Title, const FString& Description)
	{
		if (CallBack)
		{
			CallBack(Status);
		}
		if (Status == false)
		{
			TSharedRef<STapMessageBox> Box = SNew(STapMessageBox)
			.bShowCloseButton(false)
			.TitleText(FText::FromString(Title))
			.ContentRichText(FText::FromString(Description))
			.BlueButtonText(LOCTEXT("BackGame", "返回游戏"));
			UTapSubsystem::AddWidget(Box);
		}
	}, FailureHandler);
}

void AAUChinaImpl::SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack,
                                   TFunction<void(const FString& Msg)> FailureHandler)
{
	Server->SubmitPayResult(Amount, CallBack, FailureHandler);
}

FString AAUChinaImpl::CurrentToken()
{
	return Server->CurrentToken();
}

void AAUChinaImpl::TryAgainLogin(const FString& ErrMsg)
{
	FString Msg = TEXT("网络错误，请重试");
	if (!ErrMsg.IsEmpty())
	{
		Msg = ErrMsg;
	}

	TSharedRef<STapMessageBox> Box = SNew(STapMessageBox)
		.bShowCloseButton(false)
		.ContentRichText(FText::FromString(Msg))
		.BlueButtonText(LOCTEXT("Retry", "重试"))
		.OnBlueButtonClicked(FMessageBoxButtonDelegate::CreateLambda([this](const TSharedRef<class STapMessageBox>& Box)
	                                                     {
		                                                     StartupWithTapTap(CurrentUserID);
	                                                     }));
	UTapSubsystem::AddWidget(Box);
}

void AAUChinaImpl::Login(const FString& AccessToken, EAAUAgeLimit AgeLimit, bool IsFirst, bool bTapUser)
{
	FAAUUser User;
	User.AccessToken = AccessToken;
	User.AgeLimit = AgeLimit;
	User.UserID = CurrentUserID;
	TUDataStorage<FAAUStorage>::SaveStruct(FAAUStorage::HasLoginedUser + CurrentUserID, User);

	FTapCommonModule::TapThrobberShowWait();
	Server->Login(User, [=](const AAULoginResult& Result)
	{
		FTapCommonModule::TapThrobberDismiss();
		if (IsFirst && bTapUser)
		{
			if (TSharedPtr<FTULoginProfileModel> Profile = TapUELogin::GetProfile())
			{
				STapToast::Show(ETapToastType::TapTap, NSLOCTEXT("TapTap", "RealNameSuccess", "您已在 TapTap 实名！"), Profile->avatar);
			}
			else
			{
				STapToast::Show(ETapToastType::TapTap, NSLOCTEXT("TapTap", "RealNameSuccess", "您已在 TapTap 实名！"));
			}
		}
		if (Result.LoginState == AAULoginResult::SuccessWithNoLimit)
		{
			AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::LoginSuccess, "Success");
		}
		else if (Result.LoginState == AAULoginResult::SuccessWithLimit)
		{
			ShowHealthTipUI(Result.Title, Result.Description, Result.RemainTime, AAUHealthTipTypeFirstLogin);
		}
		else
		{
			if (Result.LoginState == AAULoginResult::FailWithCurfew)
			{
				ShowHealthTipUI(Result.Title, Result.Description, Result.RemainTime, AAUHealthTipTypeInCurfew);
			}
			else if (Result.LoginState == AAULoginResult::FailWithTimeout)
			{
				ShowHealthTipUI(Result.Title, Result.Description, Result.RemainTime, AAUHealthTipTypeTimeout);
			}
			else if (Result.LoginState == AAULoginResult::FailWithOther)
			{
				FTapCommonModule::TapThrobberShowToast(TEXT("防沉迷进入失败"));
				Exit();
			}
			else
			{
				FTapCommonModule::TapThrobberShowToast(TEXT("防沉迷进入失败"));
			}
		}
	});
}

void AAUChinaImpl::ShowRealNameUI(AAURealNameWordType Type, bool bFallback)
{
	const FAAUChinaAuthIdentifyWord* CurrentWordP = FAAUChinaConfigModel::GetLocalModel()->ui_config.auth_identify_words.FindByPredicate([Type](const FAAUChinaAuthIdentifyWord& EachWord)
	{
		return FCString::Atoi(*EachWord.type) == Type;
	});

	if (CurrentWordP)
	{
		switch (Type)
		{
		case AAURealNameWordTypeQuickVerify:
			{
				TWeakPtr<AAUChinaImpl> WImpl = StaticCastSharedRef<AAUChinaImpl>(AsShared());
				TSharedRef<SVerifyEntryWidget> Widget = SNew(SVerifyEntryWidget, StaticCastSharedRef<AAUChinaImpl>(AsShared()), *CurrentWordP).OnClose(FSimpleDelegate::CreateLambda([WImpl]()
				{
					if (TSharedPtr<AAUChinaImpl> Impl = WImpl.Pin())
					{
						if (!Impl->ManualVerifyWidget.IsValid())
						{
							AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::RealNameStop, "");
						}
					}
				}));
				EntryWidget = Widget;
				UTapSubsystem::AddWidget(Widget);
			}
			break;
		case AAURealNameWordTypeQuickVerifySuccess:
			break;
		case AAURealNameWordTypeQuickVerifyFail:
			STapToast::Show(ETapToastType::Error, FText::FromString(CurrentWordP->description));
			break;
		case AAURealNameWordTypeVerify:
			{
				if (TSharedPtr<SVerifyEntryWidget> Widget = EntryWidget.Pin())
				{
					UTapSubsystem::RemoveWidget(Widget.ToSharedRef());
				}
				TWeakPtr<AAUChinaImpl> WImpl = StaticCastSharedRef<AAUChinaImpl>(AsShared());
				TSharedRef<SManualVerifyWidget> Widget = SNew(SManualVerifyWidget, StaticCastSharedRef<AAUChinaImpl>(AsShared()), *CurrentWordP, bFallback).OnClose(FSimpleDelegate::CreateLambda(
					[WImpl]()
					{
						if (TSharedPtr<AAUChinaImpl> Impl = WImpl.Pin())
						{
							if (!Impl->EntryWidget.IsValid())
							{
								AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::RealNameStop, "");
							}
						}
					}));
				ManualVerifyWidget = Widget;
				UTapSubsystem::AddWidget(Widget);
			}
			break;
		case AAURealNameWordTypeVerifying:
			{
				TSharedRef<STapMessageBox> Box = SNew(STapMessageBox)
					.TitleText(FText::FromString(CurrentWordP->title))
					.ContentRichText(FText::FromString(CurrentWordP->description))
					.BlueButtonText(FText::FromString(CurrentWordP->negative_button))
					.WhiteButtonText(FText::FromString(CurrentWordP->positive_button));
				UTapSubsystem::AddWidget(Box);
			}
			break;
		case AAURealNameWordTypeVerifyFail:
			STapToast::Show(ETapToastType::Error, FText::FromString(CurrentWordP->description));
			break;
		default:
			ensure(false);
		}
	}
	else
	{
		ensure(false);
	}
}

void AAUChinaImpl::ShowHealthTipUI(const FString& Title, const FString& Content, int RemainTime, AAUHealthTipType Type)
{
	if (Type == AAUHealthTipTypeTimeout || Type == AAUHealthTipTypeInCurfew)
	{
		if (Type == AAUHealthTipTypeTimeout)
		{
			AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::DurationLimit, "");
		}
		else
		{
			AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::PeriodRestrict, "");
		}

		TSharedPtr<STapMessageBox> Box;
		if (Config.ShowSwitchAccount)
		{
			Box = SNew(STapMessageBox)
				.bShowCloseButton(false)
				.TitleText(FText::FromString(Title))
				.ContentRichText(FText::FromString(Content))
				.WhiteButtonText(LOCTEXT("QuitGame", "退出游戏"))
				.BlueButtonText(LOCTEXT("SwitchAccount", "切换账号"))
				.bCloseBoxAfterWhiteButtonClicked(false)
				.OnWhiteButtonClicked(FMessageBoxButtonDelegate::CreateLambda([](const TSharedRef<class STapMessageBox>& Box)
																 {
																	 if (UGameInstance* GI = TUSettings::GetGameInstance().Get())
																	 {
																		 UKismetSystemLibrary::QuitGame(GI, GI->GetFirstLocalPlayerController(), EQuitPreference::Quit, true);
																		 UTapSubsystem::RemoveWidget(Box);
																	 }
																	 else
																	 {
																		 UE_LOG(LogTap, Warning, TEXT("Quit game, game instance is null."));
																	 }
																 }))
				.OnBlueButtonClicked(FMessageBoxButtonDelegate::CreateLambda([](const TSharedRef<class STapMessageBox>& Box)
													 {
														 AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::SwitchAccount, "");
													 }));
		}
		else
		{
			Box = SNew(STapMessageBox)
				.bShowCloseButton(false)
				.TitleText(FText::FromString(Title))
				.ContentRichText(FText::FromString(Content))
				.BlueButtonText(LOCTEXT("QuitGame", "退出游戏"))
				.bCloseBoxAfterWhiteButtonClicked(false)
				.OnBlueButtonClicked(FMessageBoxButtonDelegate::CreateLambda([](const TSharedRef<class STapMessageBox>& Box)
																 {
																	 if (UGameInstance* GI = TUSettings::GetGameInstance().Get())
																	 {
																		 UKismetSystemLibrary::QuitGame(GI, GI->GetFirstLocalPlayerController(), EQuitPreference::Quit, true);
																		 UTapSubsystem::RemoveWidget(Box);
																	 }
																	 else
																	 {
																		 UE_LOG(LogTap, Warning, TEXT("Quit game, game instance is null."));
																	 }
																 }));
		}
		UTapSubsystem::AddWidget(Box.ToSharedRef());
	}
	else if (Type == AAUHealthTipTypeFirstLogin)
	{
		FString Temp = Content.Replace(TEXT("# ${remaining} #"), *FString::Printf(TEXT("%i"), RemainTime / 60));
		TSharedRef<STapMessageBox> Box = SNew(STapMessageBox)
				.bShowCloseButton(false)
				.TitleText(FText::FromString(Title))
				.ContentRichText(FText::FromString(Temp))
				.BlueButtonText(LOCTEXT("EnterGame", "进入游戏"))
				.OnBlueButtonClicked(FMessageBoxButtonDelegate::CreateLambda([](const TSharedRef<class STapMessageBox>& Box)
		                                                     {
			                                                     AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::LoginSuccess, "Success");
		                                                     }));
		UTapSubsystem::AddWidget(Box);
	}
}

void AAUChinaImpl::GotoTapTapQuickVerify()
{
	// Had valid token
	if (TSharedPtr<FTUAccessToken> TapToken = TapUELogin::GetAccessToken())
	{
		if (TapToken->ContainsScope(TEXT("compliance")))
		{
			QuickVerify(TapToken.ToSharedRef());
			return;
		}
	}

	StartupQuickVerifyTapAuth();
}

void AAUChinaImpl::StartupQuickVerifyTapAuth()
{
	// Request token
	FTapAuthComplete TapAuthCallback = FTapAuthComplete::CreateLambda(
		[this](const TUAuthResult& Result)
		{
			UE_LOG(LogTap, Log, TEXT("Tap quick verify, tap auth result type: %d."), Result.GetType());
			switch (Result.GetType())
			{
			case TUAuthResult::Success:
				{
					TSharedPtr<FTUAccessToken> TapToken = Result.GetToken();
					check(TapToken.IsValid());
					if (TapToken->ContainsScope(TEXT("compliance")))
					{
						QuickVerify(TapToken.ToSharedRef());
					}
					else
					{
						UE_LOG(LogTap, Warning, TEXT("Compliance auth result token doesn't contains scope 'compliance'."));
						STapToast::Show(ETapToastType::Error, LOCTEXT("AuthError", "授权错误"));
						ShowRealNameUI(AAURealNameWordTypeVerify);
					}
				}
				break;
			case TUAuthResult::Cancel:
				STapToast::Show(ETapToastType::Warning, LOCTEXT("AuthCancel", "授权取消"));
				if (!EntryWidget.IsValid())
				{
					AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::RealNameStop, "");
				}
				break;
			case TUAuthResult::Fail:
				STapToast::Show(ETapToastType::Error, LOCTEXT("AuthError", "授权错误"));
				ShowRealNameUI(AAURealNameWordTypeVerify);
				break;
			default: ;
			}
		});
	TArray<FString> Permissions{TEXT("compliance"), TUType::PermissionScope::Profile};
	UTapSubsystem::AddWidget(
		SNew(STapLoginWidget, Permissions, ERegionType::CN)
		.bRequestProfile(false)
		.OnAuthComplete(TapAuthCallback));
}

void AAUChinaImpl::QuickVerify(const TSharedRef<FTUAccessToken>& TapToken)
{
	FTapCommonModule::TapThrobberShowWait();
	AAUNet::QuickVerifyWithTapTapSmooth(CurrentUserID, TapToken, FTapTapSmoothResult::CreateSP(this, &AAUChinaImpl::HandleQuickVerifyTdsResult));
}

void AAUChinaImpl::HandleQuickVerifyTdsResult(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTapSmoothError& Error)
{
	FTapCommonModule::TapThrobberDismiss();
	if (ModelPtr.IsValid())
	{
		UE_LOG(LogTap, Log, TEXT("Tap quick verify, state: %d."), ModelPtr->GetAuthState());
		switch (ModelPtr->GetAuthState())
		{
		case FAAURealNameResultModel::Success:
			{
				if (TSharedPtr<SVerifyEntryWidget> Widget = EntryWidget.Pin())
				{
					UTapSubsystem::RemoveWidget(Widget.ToSharedRef());
				}
				Login(ModelPtr->anti_addiction_token, AAUHelper::MakeAgeLimit(ModelPtr->age_limit), true, true);
				ShowRealNameUI(AAURealNameWordTypeQuickVerifySuccess); // unused
			}
			break;
		case FAAURealNameResultModel::Verifying:
			ShowRealNameUI(AAURealNameWordTypeVerifying);
			break;
		case FAAURealNameResultModel::Fail:
			{
				STapToast::Show(ETapToastType::Error, NSLOCTEXT("TapTap", "AuthFailed", "认证未通过，请在 Tap 客户端重新提交实名信息"));
				if (!EntryWidget.IsValid() && !ManualVerifyWidget.IsValid())
				{
					AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::RealNameStop, "");
				}
			}
			break;
		}
	}
	else
	{
		UE_LOG(LogTap, Log, TEXT("Tap quick verify, code: %d, error: %s, description: %s."), Error.code, *Error.error, *Error.error_description);
		if (Error.code == 3)
		{
			int32 ErrorCode = FCString::Atoi(*Error.error);
			if (ErrorCode == 5307) // 过期
			{
				StartupQuickVerifyTapAuth();
				return;
			}
			else if (ErrorCode == 5308) // 无权限
			{
				STapToast::Show(ETapToastType::Error, LOCTEXT("AuthError", "授权错误"));
				ShowRealNameUI(AAURealNameWordTypeVerify);
				return;
			}
		}

		ShowRealNameUI(AAURealNameWordTypeVerify);
	}
}

#undef LOCTEXT_NAMESPACE