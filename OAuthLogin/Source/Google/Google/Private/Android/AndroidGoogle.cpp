// Copyright 2023 CQUnreal. All Rights Reserved.

#include "AndroidGoogle.h"
#include "GoogleModule.h"
#include "Android/AndroidJavaEnv.h"

jmethodID FAndroidGoogle::GoogleInit;
jmethodID FAndroidGoogle::GoogleLogin;
jmethodID FAndroidGoogle::GoogleLogout;

// Google初始化回调
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeGoogleInitComplete(JNIEnv* jenv, jobject thiz, jstring jdata)
{
	FString Data = TEXT("");
	if (jenv != nullptr)
	{
		Data = FJavaHelper::FStringFromParam(jenv, jdata);
	}

	UE_LOG(LogGoogle, Log, TEXT("nativeGoogleInitComplete: %s"), *Data);

	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(GOOGLE_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		LoginChannel->OnInitCompleted.ExecuteIfBound(Data);
	}
}

// Google登录回调
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeGoogleLoginComplete(JNIEnv* jenv, jobject thiz, jstring jdata)
{
	FString Data = TEXT("");
	if (jenv != nullptr)
	{
		Data = FJavaHelper::FStringFromParam(jenv, jdata);
	}

	UE_LOG(LogGoogle, Log, TEXT("nativeGoogleLoginComplete: %s"), *Data);

	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(GOOGLE_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		LoginChannel->OnLoginCompleted.ExecuteIfBound(Data);
	}
}

// Google注销登录回调
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeGoogleLogoutComplete(JNIEnv* jenv, jobject thiz, jstring jdata)
{
	FString Data = TEXT("");
	if (jenv != nullptr)
	{
		Data = FJavaHelper::FStringFromParam(jenv, jdata);
	}

	UE_LOG(LogGoogle, Log, TEXT("nativeGoogleLogoutComplete: %s"), *Data);

	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(GOOGLE_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		LoginChannel->OnLogoutCompleted.ExecuteIfBound(Data);
	}
}

FAndroidGoogle::FAndroidGoogle()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		GoogleInit = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Google_Init", "()V", false);
		GoogleLogin = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Google_Login", "()V", false);
		GoogleLogout = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Google_Logout", "()V", false);
	}
}

FAndroidGoogle::~FAndroidGoogle()
{

}

void FAndroidGoogle::Init()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, GoogleInit);
	}
}

void FAndroidGoogle::Login()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, GoogleLogin);
	}
}

void FAndroidGoogle::Logout()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, GoogleLogout);
	}
}

void FAndroidGoogle::StartupAntiAddiction()
{
	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(GOOGLE_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		LoginChannel->OnAntiAddictionEvent.ExecuteIfBound(TEXT(""));
	}
}

void FAndroidGoogle::ShutdownAntiAddiction()
{

}