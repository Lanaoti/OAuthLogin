// Copyright 2022 CQUnreal. All Rights Reserved.

#include "HaoYouKuaiBaoModule.h"
#include "OAuthLoginModule.h"
#if PLATFORM_ANDROID
#include "Android/AndroidHaoYouKuaiBao.h"
#elif PLATFORM_IOS
#include "IOS/IOSHaoYouKuaiBao.h"
#endif

/** Logging definitions */
DEFINE_LOG_CATEGORY(LogHaoYouKuaiBao);

#define LOCTEXT_NAMESPACE "FHaoYouKuaiBaoModule"

void FHaoYouKuaiBaoModule::StartupModule()
{
	UE_LOG(LogHaoYouKuaiBao, Log, TEXT("FHaoYouKuaiBaoModule::StartupModule"));

#if PLATFORM_ANDROID
	FOAuthLoginModule::Get().Register(HAOYOUKUAIBAO_CHANNEL_NAME, MakeShared<FAndroidHaoYouKuaiBao>());
#elif PLATFORM_IOS
	FOAuthLoginModule::Get().Register(HAOYOUKUAIBAO_CHANNEL_NAME, MakeShared<FIOSHaoYouKuaiBao>());
#endif
}

void FHaoYouKuaiBaoModule::ShutdownModule()
{
	UE_LOG(LogHaoYouKuaiBao, Log, TEXT("FHaoYouKuaiBaoModule::ShutdownModule"));

	FOAuthLoginModule::Get().Unregister(HAOYOUKUAIBAO_CHANNEL_NAME);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FHaoYouKuaiBaoModule, HaoYouKuaiBao)