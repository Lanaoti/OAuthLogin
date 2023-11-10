// Copyright 2023 CQUnreal. All Rights Reserved.

#pragma once

#include "OAuthLoginModule.h"

#define FACEBOOK_CHANNEL_NAME TEXT("Facebook")

DECLARE_LOG_CATEGORY_EXTERN(LogFacebook, Log, All);

class FFacebookModule : public FOAuthLoginModule
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
