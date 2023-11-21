// Copyright 2022 CQUnreal. All Rights Reserved.

#include "OAuthLoginBlueprintLibrary.h"
#include "OAuthLoginModule.h"
#include "JsonUtilities.h"


void UOAuthLoginBlueprintLibrary::Init(FOAuthInitDelegate Delegate)
{
	FOAuthLoginModule& OAuthLoginModule = FOAuthLoginModule::Get();
	OAuthLoginModule.OnInitEvent = FOnInitEvent::CreateLambda([Delegate](FName ChannelName, EOAuthResponse Code, bool bWasCompleted)
	{
		AsyncTask(ENamedThreads::GameThread, [Delegate, ChannelName, Code, bWasCompleted]()
		{
			Delegate.ExecuteIfBound(ChannelName, Code, bWasCompleted);
		});
	});
	OAuthLoginModule.Init();
}

bool UOAuthLoginBlueprintLibrary::IsEnabled(FName ChannelName)
{
	FOAuthLoginModule& OAuthLoginModule = FOAuthLoginModule::Get();
	return OAuthLoginModule.IsEnabled(ChannelName);
}

void UOAuthLoginBlueprintLibrary::Login(FName ChannelName, FOAuthLoginDelegate Delegate)
{
	UE_LOG(LogOAuthLogin, Log, TEXT("Starting Login with %s"), *ChannelName.ToString());

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

				Delegate.ExecuteIfBound(OAuthLoginData.Code, OAuthLoginData.Data);
			});
		});

		OAuthLogin->Login();
	}
	else
	{
		UE_LOG(LogOAuthLogin, Warning, TEXT("Login to %s channel not supported"), *ChannelName.ToString());

		Delegate.ExecuteIfBound(EOAuthResponse::NotSupported, FOAuthData());
	}
}

void UOAuthLoginBlueprintLibrary::Logout(FName ChannelName, FOAuthLogoutDelegate Delegate)
{
	UE_LOG(LogOAuthLogin, Log, TEXT("Starting Logout with %s"), *ChannelName.ToString());

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
	else
	{
		UE_LOG(LogOAuthLogin, Warning, TEXT("OAuthLogin to %s channel not supported"), *ChannelName.ToString());

		Delegate.ExecuteIfBound(EOAuthResponse::NotSupported);
	}
}

void UOAuthLoginBlueprintLibrary::StartupAntiAddiction(FName ChannelName, FAntiAddictionEventDelegate Delegate)
{
	UE_LOG(LogOAuthLogin, Log, TEXT("Starting StartupAntiAddiction with %s"), *ChannelName.ToString());

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

				Delegate.ExecuteIfBound(AntiAddictionEventData.Event, AntiAddictionEventData.Code, AntiAddictionEventData.Data);
			});
		});

		OAuthLogin->StartupAntiAddiction();
	}
	else
	{
		UE_LOG(LogOAuthLogin, Warning, TEXT("StartupAntiAddiction to %s channel not supported"), *ChannelName.ToString());

		Delegate.ExecuteIfBound(EAntiAddictionEvent::Startup, EOAuthResponse::SystemError, FAntiAddictionData());
	}
}

void UOAuthLoginBlueprintLibrary::ShutdownAntiAddiction(FName ChannelName)
{
	UE_LOG(LogOAuthLogin, Log, TEXT("Starting ShutdownAntiAddiction with %s"), *ChannelName.ToString());

	FOAuthLoginModule& OAuthLoginModule = FOAuthLoginModule::Get();
	FOAuthLoginPtr OAuthLogin = OAuthLoginModule.GetOAuthLogin(ChannelName);
	if (OAuthLogin.IsValid())
	{
		OAuthLogin->ShutdownAntiAddiction();
	}
	else
	{
		UE_LOG(LogOAuthLogin, Warning, TEXT("ShutdownAntiAddiction to %s channel not supported"), *ChannelName.ToString());
	}
}