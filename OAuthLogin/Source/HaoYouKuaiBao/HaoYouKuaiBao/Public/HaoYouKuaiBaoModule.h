// Copyright 2022 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#define HAOYOUKUAIBAO_CHANNEL_NAME TEXT("HaoYouKuaiBao")

DECLARE_LOG_CATEGORY_EXTERN(LogHaoYouKuaiBao, Log, All);

class FHaoYouKuaiBaoModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
