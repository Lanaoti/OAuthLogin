// Copyright 2022 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericOAuthLogin.h"

class FIOSHuaWei : public FGenericOAuthLogin
{
public:
	FIOSHuaWei();
	virtual ~FIOSHuaWei();

	virtual void Init()override;
	virtual void Login() override;
	virtual void Logout() override;
	virtual void StartupAntiAddiction() override;
	virtual void ShutdownAntiAddiction() override;
};