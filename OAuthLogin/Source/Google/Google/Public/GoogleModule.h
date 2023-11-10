// Copyright 2023 CQUnreal. All Rights Reserved.

#pragma once

#include "OAuthLoginModule.h"

#define GOOGLE_CHANNEL_NAME TEXT("Google")

DECLARE_LOG_CATEGORY_EXTERN(LogGoogle, Log, All);

class FGoogleModule : public FOAuthLoginModule
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
