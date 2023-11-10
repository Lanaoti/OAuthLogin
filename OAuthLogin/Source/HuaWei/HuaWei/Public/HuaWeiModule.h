// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "GenericHuaWei.h"

typedef TSharedPtr<FGenericHuaWei> FHuaWeiPtr;

class FHuaWeiModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static inline FHuaWeiModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FHuaWeiModule>("HuaWei");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("HuaWei");
	}

	FHuaWeiPtr GetHuaWei();
private:
	FHuaWeiPtr HuaWei;
};
