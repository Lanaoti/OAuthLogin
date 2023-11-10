#pragma once

#include "CoreMinimal.h"
#include "GenericOAuthLogin.h"

#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"

class FAndroidHaoYouKuaiBao : public FGenericOAuthLogin
{
public:
	FAndroidHaoYouKuaiBao();
	virtual ~FAndroidHaoYouKuaiBao();

	virtual void Init()override;
	virtual void Login() override;
	virtual void Logout() override;
	virtual void StartupAntiAddiction() override;
	virtual void ShutdownAntiAddiction() override;

	// JNI Methods
	static jmethodID HaoYouKuaiBaoInit;
	static jmethodID HaoYouKuaiBaoLogin;
	static jmethodID HaoYouKuaiBaoLogout;
	static jmethodID HaoYouKuaiBaoStartupAntiAddiction;
	static jmethodID HaoYouKuaiBaoShutdownAntiAddiction;
};