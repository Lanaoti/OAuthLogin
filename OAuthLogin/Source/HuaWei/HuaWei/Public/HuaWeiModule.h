// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#define HUAWEI_CHANNEL_NAME TEXT("HuaWei")

DECLARE_LOG_CATEGORY_EXTERN(LogHuaWei, Log, All);

class FHuaWeiModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
