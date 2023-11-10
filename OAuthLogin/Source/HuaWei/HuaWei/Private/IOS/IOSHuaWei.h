// Copyright 2022 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericHuaWei.h"

#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"

class FAndroidHuaWei : public FGenericHuaWei
{
public:
	FAndroidHuaWei();
	virtual ~FAndroidHuaWei();

	virtual void OnStartup() override;
	virtual void OnShutdown() override;

	virtual void Init() override;
	virtual void Login() override;
	virtual void SilentLogin() override;

	static jmethodID HuaWeiInit;
	static jmethodID HuaWeiLogin;
	static jmethodID HuaWeiSilentLogin;
};