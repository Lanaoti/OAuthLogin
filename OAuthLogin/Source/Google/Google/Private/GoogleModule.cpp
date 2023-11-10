// Copyright 2022 CQUnreal. All Rights Reserved.

#include "GoogleModule.h"
#if PLATFORM_ANDROID
#include "Android/AndroidGoogle.h"
#elif PLATFORM_IOS
#include "IOS/IOSGoogle.h"
#endif


/** Logging definitions */
DEFINE_LOG_CATEGORY(LogGoogle);

#define LOCTEXT_NAMESPACE "FGoogleModule"

void FGoogleModule::StartupModule()
{
	FOAuthLoginModule::StartupModule();

	UE_LOG(LogGoogle, Log, TEXT("FGoogleModule::StartupModule"));

#if PLATFORM_ANDROID
	Register(GOOGLE_CHANNEL_NAME, MakeShared<FAndroidGoogle>());
#elif PLATFORM_IOS
	Register(GOOGLE_CHANNEL_NAME, MakeShared<FIOSGoogle>());
#endif
}

void FGoogleModule::ShutdownModule()
{
	FOAuthLoginModule::StartupModule();

	UE_LOG(LogGoogle, Log, TEXT("FGoogleModule::ShutdownModule"));

	Unregister(GOOGLE_CHANNEL_NAME);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGoogleModule, Google)