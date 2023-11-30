// Copyright 2022 CQUnreal. All Rights Reserved.

#include "SteamModule.h"
#include "OAuthLoginModule.h"
#if PLATFORM_WINDOWS
#include "Windows/WindowsSteam.h"
#endif


/** Logging definitions */
DEFINE_LOG_CATEGORY(LogSteam);

#define LOCTEXT_NAMESPACE "FSteamModule"

void FSteamModule::StartupModule()
{
	UE_LOG(LogSteam, Log, TEXT("FSteamModule::StartupModule"));

#if PLATFORM_ANDROID
	FOAuthLoginModule::Get().Register(STEAM_CHANNEL_NAME, MakeShared<FAndroidSteam>());
#elif PLATFORM_IOS
	FOAuthLoginModule::Get().Register(STEAM_CHANNEL_NAME, MakeShared<FIOSSteam>());
#endif
}

void FSteamModule::ShutdownModule()
{
	UE_LOG(LogSteam, Log, TEXT("FSteamModule::ShutdownModule"));

	FOAuthLoginModule::Get().Unregister(STEAM_CHANNEL_NAME);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSteamModule, Steam)