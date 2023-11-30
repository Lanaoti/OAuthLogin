// Copyright 2022 CQUnreal. All Rights Reserved.

#include "FacebookModule.h"
#include "OAuthLoginModule.h"
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
	UE_LOG(LogFacebook, Log, TEXT("FFacebookModule::StartupModule"));

#if PLATFORM_ANDROID
	FOAuthLoginModule::Get().Register(FACEBOOK_CHANNEL_NAME, MakeShared<FAndroidFacebook>());
#elif PLATFORM_IOS
	FOAuthLoginModule::Get().Register(FACEBOOK_CHANNEL_NAME, MakeShared<FIOSFacebook>());
#endif
}

void FFacebookModule::ShutdownModule()
{
	UE_LOG(LogFacebook, Log, TEXT("FFacebookModule::ShutdownModule"));

	FOAuthLoginModule::Get().Unregister(FACEBOOK_CHANNEL_NAME);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFacebookModule, Facebook)