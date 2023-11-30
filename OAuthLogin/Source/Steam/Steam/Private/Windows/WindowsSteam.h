// Copyright 2023 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericOAuthLogin.h"

class FWindowsSteam : public FGenericOAuthLogin
{
public:
	FWindowsSteam();
	virtual ~FWindowsSteam();

	virtual void Init()override;
	virtual void Login() override;
	virtual void Logout() override;
	virtual void StartupAntiAddiction() override;
	virtual void ShutdownAntiAddiction() override;
};