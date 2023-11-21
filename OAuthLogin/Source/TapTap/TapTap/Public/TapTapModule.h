// Copyright 2022 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#define TAPTAP_CHANNEL_NAME TEXT("TapTap")

DECLARE_LOG_CATEGORY_EXTERN(LogTapTap, Log, All);

class FTapTapModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
