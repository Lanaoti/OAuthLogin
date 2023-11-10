// Copyright 2022 CQUnreal. All Rights Reserved.

#include "OAuthLoginBlueprintLibrary.h"
#include "OAuthLoginModule.h"
#include "JsonUtilities.h"


void UOAuthLoginBlueprintLibrary::Init()
{
	FOAuthLoginModule& OAuthLoginModule = FOAuthLoginModule::Get();
	OAuthLoginModule.Init();
}

bool UOAuthLoginBlueprintLibrary::IsEnabled(FName ChannelName)
{
	FOAuthLoginModule& OAuthLoginModule = FOAuthLoginModule::Get();
	return OAuthLoginModule.IsEnabled(ChannelName);
}

void UOAuthLoginBlueprintLibrary::Login(FName ChannelName, FOAuthLoginDelegate Delegate)
{
	FOAuthLoginModule& OAuthLoginModule = FOAuthLoginModule::Get();
	FOAuthLoginPtr OAuthLogin = OAuthLoginModule.GetOAuthLogin(ChannelName);
	if (OAuthLogin.IsValid())
	{
		OAuthLogin->OnLoginCompleted = FOnLoginCompleted::CreateLambda([Delegate](const FString& Data)
		{
			AsyncTask(ENamedThreads::GameThread, [Delegate, Data]()
			{
				FOAuthLoginData OAuthLoginData;
				FJsonObjectConverter::JsonObjectStringToUStruct<FOAuthLoginData>(Data, &OAuthLoginData);

				Delegate.ExecuteIfBound(OAuthLoginData.Code, OAuthLoginData.OAuthData);
			});
		});

		OAuthLogin->Login();
	}
	else
	{
		Delegate.ExecuteIfBound(EOAuthLoginResponse::NotSupported, FOAuthData());
	}
}

void UOAuthLoginBlueprintLibrary::Logout(FName ChannelName, FOAuthLogoutDelegate Delegate)
{
	FOAuthLoginModule& OAuthLoginModule = FOAuthLoginModule::Get();
	FOAuthLoginPtr OAuthLogin = OAuthLoginModule.GetOAuthLogin(ChannelName);
	if (OAuthLogin.IsValid())
	{
		OAuthLogin->OnLogoutCompleted = FOnLogoutCompleted::CreateLambda([Delegate](const FString& Data)
		{
			AsyncTask(ENamedThreads::GameThread, [Delegate, Data]()
			{
				FOAuthLogoutData OAuthLogoutData;
				FJsonObjectConverter::JsonObjectStringToUStruct<FOAuthLogoutData>(Data, &OAuthLogoutData);
				Delegate.ExecuteIfBound(OAuthLogoutData.Code);
			});
		});

		OAuthLogin->Logout();
	}
}

void UOAuthLoginBlueprintLibrary::StartupAntiAddiction(FName ChannelName, FAntiAddictionEventDelegate Delegate)
{
	FOAuthLoginModule& OAuthLoginModule = FOAuthLoginModule::Get();
	FOAuthLoginPtr OAuthLogin = OAuthLoginModule.GetOAuthLogin(ChannelName);
	if (OAuthLogin.IsValid())
	{
		OAuthLogin->OnAntiAddictionEvent = FOnAntiAddictionEvent::CreateLambda([Delegate](const FString& Data)
		{
			AsyncTask(ENamedThreads::GameThread, [Delegate, Data]()
			{
				FAntiAddictionEventData AntiAddictionEventData;
				FJsonObjectConverter::JsonObjectStringToUStruct<FAntiAddictionEventData>(Data, &AntiAddictionEventData);

				Delegate.ExecuteIfBound(AntiAddictionEventData.Event, AntiAddictionEventData.Code, AntiAddictionEventData.AntiAddictionData);
			});
		});

		OAuthLogin->StartupAntiAddiction();
	}
	else
	{
		Delegate.ExecuteIfBound(EAntiAddictionEvent::Init, EOAuthLoginResponse::SystemError, FAntiAddictionData());
	}
}

void UOAuthLoginBlueprintLibrary::ShutdownAntiAddiction(FName ChannelName)
{
	FOAuthLoginModule& OAuthLoginModule = FOAuthLoginModule::Get();
	FOAuthLoginPtr OAuthLogin = OAuthLoginModule.GetOAuthLogin(ChannelName);
	if (OAuthLogin.IsValid())
	{
		OAuthLogin->ShutdownAntiAddiction();
	}
}