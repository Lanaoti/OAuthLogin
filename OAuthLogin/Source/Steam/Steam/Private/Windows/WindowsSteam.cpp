// Copyright 2023 CQUnreal. All Rights Reserved.

#include "WindowsSteam.h"
#include "SteamModule.h"
#include "OAuthLoginModule.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"


#define STEAM_LOCAL_USER_NUM 0

FWindowsSteam::FWindowsSteam()
{

}

FWindowsSteam::~FWindowsSteam()
{

}

void FWindowsSteam::Init()
{
	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(STEAM_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
		if (OnlineSubsystem)
		{
			FString Data = "{ \"Code\":" + OAuthLogin::ToString(EOAuthResponse::Success) + ", \"Data\": {} }";
			LoginChannel->OnInitCompleted.ExecuteIfBound(Data);
		}
		else
		{
			FString Data = "{ \"Code\":" + OAuthLogin::ToString(EOAuthResponse::NotSupported) + ", \"Data\": {} }";
			LoginChannel->OnInitCompleted.ExecuteIfBound(Data);
		}
	}
}

FString PraseOAuthData(IOnlineSubsystem* OnlineSubsystem)
{
	check(OnlineSubsystem);

	IOnlineIdentityPtr OnlineIdentity = OnlineSubsystem->GetIdentityInterface();
	check(OnlineIdentity.IsValid());

	// 获取Steam ID
	FUniqueNetIdPtr UniqueNetId = OnlineIdentity->GetUniquePlayerId(STEAM_LOCAL_USER_NUM);
	//check(UniqueNetId.IsValid());

	FString Data = "";
	
	if (UniqueNetId.IsValid())
	{
		// 获取用户昵称
		FString Nickname = OnlineIdentity->GetPlayerNickname(*UniqueNetId);

		// 获取用户头像
		FString Avatar = TEXT("");

		// Key
		FString Key = TEXT("");

		// AccessToken
		FString AccessToken = OnlineIdentity->GetAuthToken(STEAM_LOCAL_USER_NUM);

		Data = "{ \"Code\": " + OAuthLogin::ToString(EOAuthResponse::Success) + ", \"Data\": { \
				\"UID\": \"" + UniqueNetId->ToString() + "\" , \
				\"Nickname\": \"" + Nickname + "\" , \
				\"Avatar\": \"" + Avatar + "\" , \
				\"Key\": \"" + Key + "\" , \
				\"AccessToken\": \"" + AccessToken + "\" \
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

void FWindowsSteam::Login()
{
	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(STEAM_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
		if (OnlineSubsystem)
		{
			IOnlineIdentityPtr OnlineIdentity = OnlineSubsystem->GetIdentityInterface();
			if (OnlineIdentity.IsValid())
			{
				if (OnlineIdentity->GetLoginStatus(STEAM_LOCAL_USER_NUM) == ELoginStatus::NotLoggedIn)
				{
					OnlineIdentity->AddOnLoginCompleteDelegate_Handle(STEAM_LOCAL_USER_NUM, FOnLoginCompleteDelegate::CreateLambda([&](int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error) {
						FString Data = PraseOAuthData(OnlineSubsystem);
						LoginChannel->OnLoginCompleted.ExecuteIfBound(Data);
					}));

					OnlineIdentity->AutoLogin(STEAM_LOCAL_USER_NUM);
				}
				else
				{
					FString Data = PraseOAuthData(OnlineSubsystem);
					LoginChannel->OnLoginCompleted.ExecuteIfBound(Data);
				}
			}
		}
	}
}

void FWindowsSteam::Logout()
{
	UE_LOG(LogSteam, Warning, TEXT("FWindowsSteam::Logout() unimplementable!"));
}

void FWindowsSteam::StartupAntiAddiction()
{
	UE_LOG(LogSteam, Warning, TEXT("FWindowsSteam::StartupAntiAddiction() unimplementable!"));
}

void FWindowsSteam::ShutdownAntiAddiction()
{
	UE_LOG(LogSteam, Warning, TEXT("FWindowsSteam::ShutdownAntiAddiction() unimplementable!"));
}