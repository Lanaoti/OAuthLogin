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
						FUniqueNetIdPtr UniqueNetId = OnlineIdentity->GetUniquePlayerId(STEAM_LOCAL_USER_NUM);
						check(UniqueNetId.IsValid());

						FOAuthLoginData OAuthData;
						OAuthData.Code = EOAuthResponse::Success;
						OAuthData.Data.UID = *UniqueNetId->ToString();
						OAuthData.Data.Nickname = OnlineIdentity->GetPlayerNickname(*UniqueNetId);
						OAuthData.Data.Avatar = TEXT("");
						OAuthData.Data.AuthToken = OnlineIdentity->GetAuthToken(STEAM_LOCAL_USER_NUM);

						FString Data = OAuthLogin::PraseData(OAuthData);
						LoginChannel->OnLoginCompleted.ExecuteIfBound(Data);
					}));

					OnlineIdentity->AutoLogin(STEAM_LOCAL_USER_NUM);
				}
				else
				{
					FUniqueNetIdPtr UniqueNetId = OnlineIdentity->GetUniquePlayerId(STEAM_LOCAL_USER_NUM);
					check(UniqueNetId.IsValid());

					FOAuthLoginData OAuthData;
					OAuthData.Code = EOAuthResponse::Success;
					OAuthData.Data.UID = *UniqueNetId->ToString();
					OAuthData.Data.Nickname = OnlineIdentity->GetPlayerNickname(*UniqueNetId);
					OAuthData.Data.Avatar = TEXT("");
					OAuthData.Data.AuthToken = OnlineIdentity->GetAuthToken(STEAM_LOCAL_USER_NUM);

					FString Data = OAuthLogin::PraseData(OAuthData);
					LoginChannel->OnLoginCompleted.ExecuteIfBound(Data);
				}
			}
			else
			{
				FOAuthLoginData OAuthData;
				OAuthData.Code = EOAuthResponse::SystemError;

				FString Data = OAuthLogin::PraseData(OAuthData);
				LoginChannel->OnLoginCompleted.ExecuteIfBound(Data);
			}
		}
		else
		{
			FOAuthLoginData OAuthData;
			OAuthData.Code = EOAuthResponse::NotSupported;

			FString Data = OAuthLogin::PraseData(OAuthData);
			LoginChannel->OnLoginCompleted.ExecuteIfBound(Data);
		}
	}
}

void FWindowsSteam::Logout()
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
				if (OnlineIdentity->GetLoginStatus(STEAM_LOCAL_USER_NUM) == ELoginStatus::LoggedIn)
				{
					OnlineIdentity->AddOnLogoutCompleteDelegate_Handle(STEAM_LOCAL_USER_NUM, FOnLogoutCompleteDelegate::CreateLambda([&](int32 LocalUserNum, bool bWasSuccessful) {
						FOAuthLogoutData OAuthLogoutData;
						OAuthLogoutData.Code = EOAuthResponse::Success;

						FString Data = OAuthLogin::PraseData(OAuthLogoutData);
						LoginChannel->OnLogoutCompleted.ExecuteIfBound(Data);
					}));

					OnlineIdentity->Logout(STEAM_LOCAL_USER_NUM);
				}
				else
				{
					FOAuthLogoutData OAuthLogoutData;
					OAuthLogoutData.Code = EOAuthResponse::Success;

					FString Data = OAuthLogin::PraseData(OAuthLogoutData);
					LoginChannel->OnLogoutCompleted.ExecuteIfBound(Data);
				}
			}
		}
	}
}

void FWindowsSteam::StartupAntiAddiction()
{
	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(STEAM_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		FAntiAddictionEventData AntiAddictionEventData;
		AntiAddictionEventData.Event = EAntiAddictionEvent::Startup;
		AntiAddictionEventData.Code = EOAuthResponse::Success;
		
		FString Data = OAuthLogin::PraseData(AntiAddictionEventData);
		LoginChannel->OnAntiAddictionEvent.ExecuteIfBound(Data);
	}
}

void FWindowsSteam::ShutdownAntiAddiction()
{
	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(STEAM_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		FAntiAddictionEventData AntiAddictionEventData;
		AntiAddictionEventData.Event = EAntiAddictionEvent::Shutdown;
		AntiAddictionEventData.Code = EOAuthResponse::Success;

		FString Data = OAuthLogin::PraseData(AntiAddictionEventData);
		LoginChannel->OnAntiAddictionEvent.ExecuteIfBound(Data);
	}
}