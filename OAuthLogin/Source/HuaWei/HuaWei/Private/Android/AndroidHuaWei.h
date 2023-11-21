#pragma once

#include "CoreMinimal.h"
#include "GenericOAuthLogin.h"

#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"

class FAndroidHuaWei : public FGenericOAuthLogin
{
public:
	FAndroidHuaWei();
	virtual ~FAndroidHuaWei();

	virtual void Init()override;
	virtual void Login() override;
	virtual void Logout() override;
	virtual void StartupAntiAddiction() override;
	virtual void ShutdownAntiAddiction() override;

	// JNI Methods
	static jmethodID HuaWeiInit;
	static jmethodID HuaWeiLogin;
	static jmethodID HuaWeiLogout;
	static jmethodID HuaWeiStartupAntiAddiction;
	static jmethodID HuaWeiShutdownAntiAddiction;
};