#include "AndroidHuaWei.h"
#include "OAuthLoginModule.h"
#include "HuaWeiModule.h"
#include "Android/AndroidJavaEnv.h"

jmethodID FAndroidHuaWei::HuaWeiInit;
jmethodID FAndroidHuaWei::HuaWeiLogin;
jmethodID FAndroidHuaWei::HuaWeiLogout;
jmethodID FAndroidHuaWei::HuaWeiStartupAntiAddiction;
jmethodID FAndroidHuaWei::HuaWeiShutdownAntiAddiction;

// HuaWei初始化回调
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeHuaWeiInitComplete(JNIEnv* jenv, jobject thiz, jstring msg)
{
	FString Data = TEXT("");
	if (jenv != nullptr)
	{
		Data = FJavaHelper::FStringFromParam(jenv, msg);
	}

	UE_LOG(LogHuaWei, Log, TEXT("nativeHuaWeiInitComplete: %s"), *Data);

	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(HUAWEI_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		LoginChannel->OnInitCompleted.ExecuteIfBound(Data);
	}
}

// HuaWei登录回调
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeHuaWeiLoginComplete(JNIEnv* jenv, jobject thiz, jstring msg)
{
	FString Data = TEXT("");
	if (jenv != nullptr)
	{
		Data = FJavaHelper::FStringFromParam(jenv, msg);
	}

	UE_LOG(LogHuaWei, Log, TEXT("nativeHuaWeiLoginComplete: %s"), *Data);

	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(HUAWEI_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		LoginChannel->OnLoginCompleted.ExecuteIfBound(Data);
	}
}

// HuaWei注销登录回调
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeHuaWeiLogoutComplete(JNIEnv* jenv, jobject thiz, jstring jdata)
{
	FString Data = TEXT("");
	if (jenv != nullptr)
	{
		Data = FJavaHelper::FStringFromParam(jenv, jdata);
	}

	UE_LOG(LogHuaWei, Log, TEXT("nativeHuaWeiLogoutComplete: %s"), *Data);

	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(HUAWEI_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		LoginChannel->OnLogoutCompleted.ExecuteIfBound(Data);
	}
}

// HuaWei防沉迷回调
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeHuaWeiAntiAddictionEvent(JNIEnv* jenv, jobject thiz, jstring msg)
{
	FString Data = TEXT("");
	if (jenv != nullptr)
	{
		Data = FJavaHelper::FStringFromParam(jenv, msg);
	}

	UE_LOG(LogHuaWei, Log, TEXT("nativeHuaWeiAntiAddictionEvent: %s"), *Data);

	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(HUAWEI_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		LoginChannel->OnAntiAddictionEvent.ExecuteIfBound(Data);
	}
}

FAndroidHuaWei::FAndroidHuaWei()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		HuaWeiInit = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_HuaWei_Init", "()V", false);
		HuaWeiLogin = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_HuaWei_Login", "()V", false);
		HuaWeiLogout = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_HuaWei_Logout", "()V", false);
		HuaWeiStartupAntiAddiction = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_HuaWei_StartupAntiAddiction", "()V", false);
		HuaWeiShutdownAntiAddiction = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_HuaWei_ShutdownAntiAddiction", "()V", false);
	}
}

FAndroidHuaWei::~FAndroidHuaWei()
{

}

void FAndroidHuaWei::Init()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaWeiInit);
	}
}

void FAndroidHuaWei::Login()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaWeiLogin);
	}
}

void FAndroidHuaWei::Logout()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaWeiLogout);
	}
}

void FAndroidHuaWei::StartupAntiAddiction()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaWeiStartupAntiAddiction);
	}
}

void FAndroidHuaWei::ShutdownAntiAddiction()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaWeiShutdownAntiAddiction);
	}
}