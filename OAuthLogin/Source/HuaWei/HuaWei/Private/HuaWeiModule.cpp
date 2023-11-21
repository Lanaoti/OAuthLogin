// Copyright Epic Games, Inc. All Rights Reserved.

#include "HuaWeiModule.h"
#include "OAuthLoginModule.h"
#if PLATFORM_ANDROID
#include "Android/AndroidHuaWei.h"
#elif PLATFORM_IOS
#include "IOS/IOSHuaWei.h"
#endif

/** Logging definitions */
DEFINE_LOG_CATEGORY(LogHuaWei);

#define LOCTEXT_NAMESPACE "FHuaWeiModule"

void FHuaWeiModule::StartupModule()
{
	UE_LOG(LogHuaWei, Log, TEXT("FHuaWeiModule::StartupModule"));

#if PLATFORM_ANDROID
	FOAuthLoginModule::Get().Register(HUAWEI_CHANNEL_NAME, MakeShared<FAndroidHuaWei>());
#elif PLATFORM_IOS
	FOAuthLoginModule::Get().Register(HUAWEI_CHANNEL_NAME, MakeShared<FIOSHuaWei>());
#endif
}

void FHuaWeiModule::ShutdownModule()
{
	UE_LOG(LogHuaWei, Log, TEXT("FHuaWeiModule::ShutdownModule"));

	FOAuthLoginModule::Get().Unregister(HUAWEI_CHANNEL_NAME);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHuaWeiModule, HuaWei)