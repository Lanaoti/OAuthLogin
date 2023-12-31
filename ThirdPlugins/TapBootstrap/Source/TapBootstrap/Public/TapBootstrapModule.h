// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class TAPBOOTSTRAP_API FTapBootstrapModule : public IModuleInterface
{
public:
	
protected:
    virtual void StartupModule() override;
    
    virtual void ShutdownModule() override;

};

