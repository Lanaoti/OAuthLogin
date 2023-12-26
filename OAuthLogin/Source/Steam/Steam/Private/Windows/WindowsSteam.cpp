// Copyright 2023 CQUnreal. All Rights Reserved.

#include "WindowsSteam.h"
#include "SteamModule.h"
#include "OnlineSubsystem.h"


void FWindowsSteam::Init()
{
	UE_LOG(LogSteam, Warning, TEXT("FWindowsSteam::Init() unimplementable!"));

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
}

void FWindowsSteam::Login()
{
	UE_LOG(LogSteam, Warning, TEXT("FWindowsSteam::Login() unimplementable!"));
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