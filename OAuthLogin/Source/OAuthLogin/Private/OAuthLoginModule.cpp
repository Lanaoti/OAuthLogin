// Copyright 2023 CQUnreal. All Rights Reserved.

#include "OAuthLoginModule.h"

/** Logging definitions */
DEFINE_LOG_CATEGORY(LogOAuthLogin);

#define LOCTEXT_NAMESPACE "FOAuthLoginModule"

TMap<FName, FOAuthLoginRef> FOAuthLoginModule::LoginChannels;

void FOAuthLoginModule::StartupModule()
{

}

void FOAuthLoginModule::ShutdownModule()
{

}

FOAuthLoginPtr FOAuthLoginModule::GetOAuthLogin(FName ModuleName)
{
	FOAuthLoginRef* OAuthLogin = LoginChannels.Find(ModuleName);
	if (OAuthLogin != nullptr)
	{
		return *OAuthLogin;
	}

	return nullptr;
}

void FOAuthLoginModule::Init()
{
	for (TPair<FName, FOAuthLoginRef> Pair : LoginChannels)
	{
		Pair.Value->Init();
	}
}

bool FOAuthLoginModule::IsEnabled(FName ChannelName) const
{
	return LoginChannels.Contains(ChannelName);
}

void FOAuthLoginModule::Register(FName ChannelName, FOAuthLoginRef LoginChannel)
{
	LoginChannels.Add(ChannelName, LoginChannel);
}

void FOAuthLoginModule::Unregister(FName ChannelName)
{
	LoginChannels.Remove(ChannelName);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FOAuthLoginModule, OAuthLogin)