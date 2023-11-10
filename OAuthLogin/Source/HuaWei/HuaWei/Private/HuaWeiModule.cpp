// Copyright Epic Games, Inc. All Rights Reserved.

#include "HuaWeiModule.h"
#include "HuaWeiManager.h"
#if PLATFORM_ANDROID
#include "Android/AndroidHuaWei.h"
#elif PLATFORM_IOS
#endif


#define LOCTEXT_NAMESPACE "FHuaWeiModule"

void FHuaWeiModule::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("FHuaWeiModule::StartupModule"));

#if PLATFORM_ANDROID
	HuaWei = MakeShared<FAndroidHuaWei>();
#elif PLATFORM_IOS

#endif

	if (HuaWei.IsValid())
	{
		HuaWei->OnStartup();
	}

	UHuaWeiManager::Get()->Init();
}

void FHuaWeiModule::ShutdownModule()
{
	if (HuaWei.IsValid())
	{
		HuaWei->OnShutdown();
	}
	UHuaWeiManager::Get()->Shutdown();
}
FHuaWeiPtr FHuaWeiModule::GetHuaWei()
{
	return HuaWei;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHuaWeiModule, HuaWei)