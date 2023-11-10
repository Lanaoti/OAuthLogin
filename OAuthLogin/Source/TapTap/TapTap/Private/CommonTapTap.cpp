// Copyright 2023 CQUnreal. All Rights Reserved.

#include "CommonTapTap.h"
#include "TapTapModule.h"
#include "TapUECommon.h"
#include "TapUELogin.h"
#include "AntiAddictionUE.h"
#include "TUAuthResult.h"
#include "TULoginProfileModel.h"


static bool bStartupAntiAddiction = false;
static bool bIsInitializedTapTapSDK = false;

FCommonTapTap::FCommonTapTap()
{

}

FCommonTapTap::~FCommonTapTap()
{

}

void FCommonTapTap::Init()
{
	if (bIsInitializedTapTapSDK)
	{
		return;
	}

	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddRaw(this, &FCommonTapTap::HandleAppSuspend);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddRaw(this, &FCommonTapTap::HandleAppResume);
	FCoreDelegates::ApplicationWillDeactivateDelegate.AddRaw(this, &FCommonTapTap::WillDeactivate);
	FCoreDelegates::ApplicationHasReactivatedDelegate.AddRaw(this, &FCommonTapTap::HasReactivated);

	FString ClientID;
	GConfig->GetString(TEXT("TapTap"), TEXT("ClientID"), ClientID, GGameIni);

	FString RegionTypeStr;
	GConfig->GetString(TEXT("TapTap"), TEXT("RegionType"), RegionTypeStr, GGameIni);
	UEnum* RegionTypeEnum = StaticEnum<ERegionType>();
	ERegionType RegionType = (ERegionType)RegionTypeEnum->GetValueByName(*RegionTypeStr);

	FString LanguageTypeStr;
	GConfig->GetString(TEXT("TapTap"), TEXT("LanguageType"), LanguageTypeStr, GGameIni);
	UEnum* LanguageTypeEnum = StaticEnum<ELanguageType>();
	ELanguageType LanguageType = (ELanguageType)RegionTypeEnum->GetValueByName(*RegionTypeStr);

	FString ClientToken;
	GConfig->GetString(TEXT("TapTap"), TEXT("ClientToken"), ClientToken, GGameIni);

	FString ServerURL;
	GConfig->GetString(TEXT("TapTap"), TEXT("ServerURL"), ServerURL, GGameIni);

	TapUECommon::SetLanguage(LanguageType);

	// 初始化公共模块
	FTUConfig TUConfig;
	TUConfig.ClientID = ClientID;
	TUConfig.RegionType = RegionType;
	TUConfig.ClientToken = ClientToken;
	TUConfig.ServerURL = ServerURL;
	FTUConfig::Init(TUConfig);

	// 初始化登录模块
	FTULoginConfig TULoginConfig;
	TULoginConfig.ClientID = ClientID;
	TULoginConfig.RegionType = RegionType;
	TULoginConfig.bRoundCorner = true;
	TapUELogin::Init(TULoginConfig);

	// 初始化反沉迷模块
	FAAUConfig AAUConfig;
	AAUConfig.ClientID = ClientID;
	AAUConfig.UseTapLogin = false;			// 暂不支持
	AAUConfig.ShowSwitchAccount = false;
	AAUConfig.Region = EAAURegion::China;
	AntiAddictionUE::Init(AAUConfig);

	bIsInitializedTapTapSDK = true;
}

void FCommonTapTap::Login()
{
LoginPre:
	TSharedPtr<FTUAccessToken> TUAccessToken = TapUELogin::GetAccessToken();
	TSharedPtr<FTULoginProfileModel> TULoginProfileModel = FTapTapModule::Get().GetProfile();
	if (TUAccessToken.IsValid() && TULoginProfileModel.IsValid())
	{
		Unionid = TULoginProfileModel->unionid;

		FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(GOOGLE_CHANNEL_NAME);
		if (LoginChannel.IsValid())
		{
			FString Data = "{ \"Code:\"" + (int32)EOAuthLoginResponse::Success + ", \"Data\": { \
					\"UID\": \"" + TULoginProfileModel->unionid + "\" , \
					\"Nickname\": \"" + TULoginProfileModel->name + "\" , \
					\"Avator\": \"" + TULoginProfileModel->avatar + "\" , \
					\"Key\": \"" + TUAccessToken->mac_key + "\" , \
					\"AccessToken\": \"" + TUAccessToken->token_type + "\" \
				} \
			}";
			LoginChannel->OnLoginCompleted.ExecuteIfBound(Data);
		}
	}
	else
	{
		TapUELogin::Login([](const TUAuthResult& Result) {
			FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(GOOGLE_CHANNEL_NAME);
			if (LoginChannel.IsValid())
			{
				TSharedPtr<FTUError> TUError = Result.GetError();
				if (TUError.IsValid())
				{
					UE_LOG(LogTapTap, Warning, TEXT("TapTap login failed: %d"), TUError->code);

					FString Data = "{ \"Code:\"" + (int32)EOAuthLoginResponse::SystemError + ", \"Data\": { \
							\"UID\": \"\" , \
							\"Nickname\": \"\" , \
							\"Avator\": \"\" , \
							\"Key\": \"\" , \
							\"AccessToken\": \"\" \
						} \
					}";
					LoginChannel->OnLoginCompleted.ExecuteIfBound(Data);
				}
				else
				{
					goto LoginPre;
				}
			}
		});
	}
}

void FCommonTapTap::Logout()
{
	TapUELogin::Logout();

	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(GOOGLE_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		FString Data = "{ \"Code:\"" + (int32)EOAuthLoginResponse::Success + " }";
		LoginChannel->OnLogoutCompleted.ExecuteIfBound(Data);
	}
}

void FCommonTapTap::StartupAntiAddiction()
{
	AntiAddictionUE::OnCallBack = [](ResultHandlerCode Code, const FString& Data) {
		FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(TAPTAP_CHANNEL_NAME);
		if (LoginChannel.IsValid())
		{
			FString Data = "";
			if (Code == ResultHandlerCode::LoginSuccess)
			{
				Data = { \"Event:\" \"" + (int32)EAntiAddictionEvent::Init + "\", \"Code:\"" + (int32)EOAuthLoginResponse::Success + ", \"Data\": {}";
			}
			else
			{
				Data = { \"Event:\" \"" + (int32)EAntiAddictionEvent::Init + "\", \"Code:\"" + (int32)EOAuthLoginResponse::SystemError + ", \"Data\": {}";
			}

			LoginChannel->OnAntiAddictionEvent.ExecuteIfBound(Data);
		}
	};

	AntiAddictionUE::Startup(Unionid, true);
}

void FCommonTapTap::ShutdownAntiAddiction()
{
	AntiAddictionUE::Exit();
}

void FCommonTapTap::WillDeactivate()
{
	if (bStartupAntiAddiction)
	{
		AntiAddictionUE::LeaveGame();
	}
}

void FCommonTapTap::HasReactivated()
{
	if (bStartupAntiAddiction)
	{
		AntiAddictionUE::EnterGame();
	}
}

void FCommonTapTap::HandleAppSuspend()
{
	if (bStartupAntiAddiction)
	{
		AntiAddictionUE::LeaveGame();
	}
}

void FCommonTapTap::HandleAppResume()
{
	if (bStartupAntiAddiction)
	{
		AntiAddictionUE::EnterGame();
	}
}
