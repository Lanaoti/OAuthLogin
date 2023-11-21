#include "AndroidHaoYouKuaiBao.h"
#include "OAuthLoginModule.h"
#include "HaoYouKuaiBaoModule.h"
#include "Android/AndroidJavaEnv.h"

jmethodID FAndroidHaoYouKuaiBao::HaoYouKuaiBaoInit;
jmethodID FAndroidHaoYouKuaiBao::HaoYouKuaiBaoLogin;
jmethodID FAndroidHaoYouKuaiBao::HaoYouKuaiBaoLogout;
jmethodID FAndroidHaoYouKuaiBao::HaoYouKuaiBaoStartupAntiAddiction;
jmethodID FAndroidHaoYouKuaiBao::HaoYouKuaiBaoShutdownAntiAddiction;

// 好游快爆初始化回调
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeHaoYouKuaiBaoInitComplete(JNIEnv* jenv, jobject thiz, jstring msg)
{
	FString Data = TEXT("");
	if (jenv != nullptr)
	{
		Data = FJavaHelper::FStringFromParam(jenv, msg);
	}

	UE_LOG(LogHaoYouKuaiBao, Log, TEXT("nativeHaoYouKuaiBaoInitComplete: %s"), *Data);

	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(HAOYOUKUAIBAO_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		LoginChannel->OnInitCompleted.ExecuteIfBound(Data);
	}
}

// 好游快爆登录回调
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeHaoYouKuaiBaoLoginComplete(JNIEnv* jenv, jobject thiz, jstring msg)
{
	FString Data = TEXT("");
	if (jenv != nullptr)
	{
		Data = FJavaHelper::FStringFromParam(jenv, msg);
	}

	UE_LOG(LogHaoYouKuaiBao, Log, TEXT("nativeHaoYouKuaiBaoLoginComplete: %s"), *Data);

	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(HAOYOUKUAIBAO_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		LoginChannel->OnLoginCompleted.ExecuteIfBound(Data);
	}
}

// 好游快爆注销登录回调
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeHaoYouKuaiBaoLogoutComplete(JNIEnv* jenv, jobject thiz, jstring jdata)
{
	FString Data = TEXT("");
	if (jenv != nullptr)
	{
		Data = FJavaHelper::FStringFromParam(jenv, jdata);
	}

	UE_LOG(LogHaoYouKuaiBao, Log, TEXT("nativeHaoYouKuaiBaoLogoutComplete: %s"), *Data);

	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(HAOYOUKUAIBAO_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		LoginChannel->OnLogoutCompleted.ExecuteIfBound(Data);
	}
}

// 好游快爆防沉迷回调
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeHaoYouKuaiBaoAntiAddictionEvent(JNIEnv* jenv, jobject thiz, jstring msg)
{
	FString Data = TEXT("");
	if (jenv != nullptr)
	{
		Data = FJavaHelper::FStringFromParam(jenv, msg);
	}

	UE_LOG(LogHaoYouKuaiBao, Log, TEXT("nativeHaoYouKuaiBaoAntiAddictionEvent: %s"), *Data);

	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(HAOYOUKUAIBAO_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		LoginChannel->OnAntiAddictionEvent.ExecuteIfBound(Data);
	}
}

FAndroidHaoYouKuaiBao::FAndroidHaoYouKuaiBao()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		HaoYouKuaiBaoInit = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_HaoYouKuaiBao_Init", "()V", false);
		HaoYouKuaiBaoLogin = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_HaoYouKuaiBao_Login", "()V", false);
		HaoYouKuaiBaoLogout = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_HaoYouKuaiBao_Logout", "()V", false);
		HaoYouKuaiBaoStartupAntiAddiction = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_HaoYouKuaiBao_StartupAntiAddiction", "()V", false);
		HaoYouKuaiBaoShutdownAntiAddiction = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_HaoYouKuaiBao_ShutdownAntiAddiction", "()V", false);
	}
}

FAndroidHaoYouKuaiBao::~FAndroidHaoYouKuaiBao()
{

}

void FAndroidHaoYouKuaiBao::Init()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HaoYouKuaiBaoInit);
	}
}

void FAndroidHaoYouKuaiBao::Login()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HaoYouKuaiBaoLogin);
	}
}

void FAndroidHaoYouKuaiBao::Logout()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HaoYouKuaiBaoLogout);
	}
}

void FAndroidHaoYouKuaiBao::StartupAntiAddiction()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HaoYouKuaiBaoStartupAntiAddiction);
	}
}

void FAndroidHaoYouKuaiBao::ShutdownAntiAddiction()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HaoYouKuaiBaoShutdownAntiAddiction);
	}
}