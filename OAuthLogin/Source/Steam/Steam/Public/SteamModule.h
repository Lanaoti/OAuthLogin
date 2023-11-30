// Copyright 2023 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#define STEAM_CHANNEL_NAME TEXT("Steam")

DECLARE_LOG_CATEGORY_EXTERN(LogSteam, Log, All);

class FSteamModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
