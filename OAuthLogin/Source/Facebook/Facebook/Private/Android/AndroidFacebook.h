// Copyright 2023 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericOAuthLogin.h"

#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"

class FAndroidFacebook : public FGenericOAuthLogin
{
public:
	FAndroidFacebook();
	virtual ~FAndroidFacebook();

	virtual void Init()override;
	virtual void Login() override;
	virtual void Logout() override;
	virtual void StartupAntiAddiction() override;
	virtual void ShutdownAntiAddiction() override;

	// JNI Methods
	static jmethodID FacebookInit;
	static jmethodID FacebookLogin;
	static jmethodID FacebookLogout;
};