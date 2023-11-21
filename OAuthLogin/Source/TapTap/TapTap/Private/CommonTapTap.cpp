// Copyright 2023 CQUnreal. All Rights Reserved.

#include "CommonTapTap.h"
#include "OAuthLoginModule.h"
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
	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(TAPTAP_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		if (bIsInitializedTapTapSDK)
		{
			FString Data = "{ \"Code\":" + OAuthLogin::ToString(EOAuthResponse::Success) + ", \"Data\": {} }";
			LoginChannel->OnInitCompleted.ExecuteIfBound(Data);
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

		FString Data = "{ \"Code\":" + OAuthLogin::ToString(EOAuthResponse::Success) + ", \"Data\": {} }";
		LoginChannel->OnInitCompleted.ExecuteIfBound(Data);
	}
	else
	{
		FString Data = "{ \"Code\":" + OAuthLogin::ToString(EOAuthResponse::NotSupported) + ", \"Data\": {} }";
		LoginChannel->OnInitCompleted.ExecuteIfBound(Data);
	}
}

FString PraseOAuthData(TSharedPtr<FTUAccessToken> TUAccessToken, TSharedPtr<FTULoginProfileModel> TULoginProfileModel)
{
	FString Data = "";

	if (TUAccessToken.IsValid() && TULoginProfileModel.IsValid())
	{
		Data = "{ \"Code\": " + OAuthLogin::ToString(EOAuthResponse::Success) + ", \"Data\": { \
				\"UID\": \"" + TULoginProfileModel->unionid + "\" , \
				\"Nickname\": \"" + TULoginProfileModel->name + "\" , \
				\"Avatar\": \"" + TULoginProfileModel->avatar + "\" , \
				\"Key\": \"" + TUAccessToken->mac_key + "\" , \
				\"AccessToken\": \"" + TUAccessToken->access_token + "\" \
			} \
		}";
	}
	else
	{
		Data = "{ \"Code\":" + OAuthLogin::ToString(EOAuthResponse::SystemError) + ", \"Data\": { \
				\"UID\": \"\" , \
				\"Nickname\": \"\" , \
				\"Avatar\": \"\" , \
				\"Key\": \"\" , \
				\"AccessToken\": \"\" \
			} \
		}";
	}

	return Data;
}

void FCommonTapTap::Login()
{
	TSharedPtr<FTUAccessToken> TUAccessToken = TapUELogin::GetAccessToken();
	TSharedPtr<FTULoginProfileModel> TULoginProfileModel = TapUELogin::GetProfile();
	if (TUAccessToken.IsValid() && TULoginProfileModel.IsValid())
	{
		Unionid = TULoginProfileModel->unionid;

		FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(TAPTAP_CHANNEL_NAME);
		if (LoginChannel.IsValid())
		{
			FString Data = PraseOAuthData(TUAccessToken, TULoginProfileModel);
			LoginChannel->OnLoginCompleted.ExecuteIfBound(Data);
		}
	}
	else
	{
		TapUELogin::Login([&](const TUAuthResult& Result) {
			FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(TAPTAP_CHANNEL_NAME);
			if (LoginChannel.IsValid())
			{
				TSharedPtr<FTUError> TUError = Result.GetError();
				if (TUError.IsValid())
				{
					UE_LOG(LogTapTap, Warning, TEXT("TapTap login failed: %d"), TUError->code);

					FString Data = PraseOAuthData(nullptr, nullptr);
					LoginChannel->OnLoginCompleted.ExecuteIfBound(Data);
				}
				else
				{
					TSharedPtr<FTUAccessToken> TUAccessToken = TapUELogin::GetAccessToken();
					TSharedPtr<FTULoginProfileModel> TULoginProfileModel = TapUELogin::GetProfile();
					if (TUAccessToken.IsValid() && TULoginProfileModel.IsValid())
					{
						Unionid = TULoginProfileModel->unionid;

						FString Data = PraseOAuthData(TUAccessToken, TULoginProfileModel);
						LoginChannel->OnLoginCompleted.ExecuteIfBound(Data);
					}
					else
					{
						UE_LOG(LogTapTap, Warning, TEXT("TapTap login get AccessToken/ProfileModel failed"));

						FString Data = PraseOAuthData(nullptr, nullptr);
						LoginChannel->OnLoginCompleted.ExecuteIfBound(Data);
					}
				}
			}
		});
	}	
}

void FCommonTapTap::Logout()
{
	TapUELogin::Logout();

	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(TAPTAP_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		FString Data = "{ \"Code\":" + OAuthLogin::ToString(EOAuthResponse::Success) + " }";
		LoginChannel->OnLogoutCompleted.ExecuteIfBound(Data);
	}
}

void FCommonTapTap::StartupAntiAddiction()
{
	AntiAddictionUE::OnCallBack = AntiAddictionUE::FCallBack::CreateLambda([](AntiAddictionUE::ResultHandlerCode Code, const FString& Message) {
		FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(TAPTAP_CHANNEL_NAME);
		if (LoginChannel.IsValid())
		{
			FString Data = "";
			if (Code == AntiAddictionUE::ResultHandlerCode::LoginSuccess)
			{
				Data = "{\"Event\":" + OAuthLogin::ToString(EAntiAddictionEvent::Startup) + ", \"Code\": " + OAuthLogin::ToString(EOAuthResponse::Success) + ", \"Data\": {} }";
			}
			else
			{
				Data = "{\"Event\":" + OAuthLogin::ToString(EAntiAddictionEvent::Startup) + ", \"Code\": " + OAuthLogin::ToString(EOAuthResponse::SystemError) + ", \"Data\": {} }";
			}

			LoginChannel->OnAntiAddictionEvent.ExecuteIfBound(Data);
		}
	});

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
