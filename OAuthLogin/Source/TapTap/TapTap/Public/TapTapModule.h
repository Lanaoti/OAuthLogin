// Copyright 2022 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OAuthLoginModule.h"

#define TAPTAP_CHANNEL_NAME TEXT("Google")

DECLARE_LOG_CATEGORY_EXTERN(LogTapTap, Log, All);

class FTapTapModule : public FOAuthLoginModule
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
