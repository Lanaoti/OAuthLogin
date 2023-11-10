// Copyright 2023 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericOAuthLogin.h"

class FCommonTapTap : public FGenericOAuthLogin
{
public:
	FCommonTapTap();
	virtual ~FCommonTapTap();

	virtual void Init()override;
	virtual void Login() override;
	virtual void Logout() override;
	virtual void StartupAntiAddiction() override;
	virtual void ShutdownAntiAddiction() override;

private:
	// This is called when the application is about to be deactivated (e.g., due to a phone call or SMS or the sleep button).
	void WillDeactivate();

	// Called when the application has been reactivated (reverse any processing done in the Deactivate delegate)
	void HasReactivated();

	// Callback occurs when game being suspended
	void HandleAppSuspend();

	// Callback occurs when game resuming
	void HandleAppResume();

private:
	FString Unionid;
};