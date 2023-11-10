// Copyright 2022 CQUnreal. All Rights Reserved.

#include "FacebookModule.h"
#if PLATFORM_ANDROID
#include "Android/AndroidFacebook.h"
#elif PLATFORM_IOS
#include "IOS/IOSFacebook.h"
#endif


/** Logging definitions */
DEFINE_LOG_CATEGORY(LogFacebook);

#define LOCTEXT_NAMESPACE "FFacebookModule"

void FFacebookModule::StartupModule()
{
	FOAuthLoginModule::StartupModule();

	UE_LOG(LogFacebook, Log, TEXT("FFacebookModule::StartupModule"));

#if PLATFORM_ANDROID
	Register(FACEBOOK_CHANNEL_NAME, MakeShared<FAndroidFacebook>());
#elif PLATFORM_IOS
	Register(FACEBOOK_CHANNEL_NAME, MakeShared<FIOSFacebook>());
#endif
}

void FFacebookModule::ShutdownModule()
{
	FOAuthLoginModule::StartupModule();

	UE_LOG(LogFacebook, Log, TEXT("FFacebookModule::ShutdownModule"));

	Unregister(FACEBOOK_CHANNEL_NAME);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFacebookModule, Facebook)