// Copyright 2022 CQUnreal. All Rights Reserved.

#include "TapTapModule.h"
#include "CommonTapTap.h"


/** Logging definitions */
DEFINE_LOG_CATEGORY(LogTapTap);

#define LOCTEXT_NAMESPACE "FTapTapModule"

void FTapTapModule::StartupModule()
{
	FOAuthLoginModule::StartupModule();

	UE_LOG(LogTapTap, Log, TEXT("FTapTapModule::StartupModule"));

	Register(TAPTAP_CHANNEL_NAME, MakeShared<FCommonTapTap>());
}

void FTapTapModule::ShutdownModule()
{
	FOAuthLoginModule::StartupModule();

	UE_LOG(LogTapTap, Log, TEXT("FTapTapModule::ShutdownModule"));

	Unregister(TAPTAP_CHANNEL_NAME);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTapTapModule, TapTap)