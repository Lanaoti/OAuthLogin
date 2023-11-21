// Copyright 2023 CQUnreal. All Rights Reserved.

#include "AndroidFacebook.h"
#include "Android/AndroidJavaEnv.h"
#include "FacebookModule.h"

jmethodID FAndroidFacebook::FacebookInit;
jmethodID FAndroidFacebook::FacebookLogin;
jmethodID FAndroidFacebook::FacebookLogout;

// Facebook登录回调
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeFacebookLoginComplete(JNIEnv* jenv, jobject thiz, jstring jdata)
{
	FString Data = TEXT("");
	if (jenv != nullptr)
	{
		Data = FJavaHelper::FStringFromParam(jenv, jdata);
	}

	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(FACEBOOK_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		LoginChannel->OnLoginCompleted.ExecuteIfBound(Data);
	}

	UE_LOG(LogFacebook, Log, TEXT("nativeFacebookLoginComplete: %s"), *Data);
}

// Facebook注销登录回调
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeFacebookLogoutComplete(JNIEnv* jenv, jobject thiz, jstring jdata)
{
	FString Data = TEXT("");
	if (jenv != nullptr)
	{
		Data = FJavaHelper::FStringFromParam(jenv, jdata);
	}

	UE_LOG(LogFacebook, Log, TEXT("nativeFacebookLogoutComplete: %s"), *Data);
}

FAndroidFacebook::FAndroidFacebook()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FacebookInit = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Facebook_Init", "()V", false);
		FacebookLogin = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Facebook_Login", "()V", false);
		FacebookLogout = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Facebook_Logout", "()V", false);
	}
}

FAndroidFacebook::~FAndroidFacebook()
{

}

void FAndroidFacebook::Init()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, FacebookInit);
	}
}

void FAndroidFacebook::Login()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, FacebookLogin);
	}
}

void FAndroidFacebook::Logout()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, FacebookLogout);
	}
}

void FAndroidFacebook::StartupAntiAddiction()
{
	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(GOOGLE_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		LoginChannel->OnAntiAddictionEvent.ExecuteIfBound(TEXT("{ \"Event\": " + OAuthLogin::ToString(EAntiAddictionEvent::Startup) + ", \"Code\": " + OAuthLogin::ToString(EOAuthResponse::Success) + ", \"Data\": {} }"));
	}
}

void FAndroidFacebook::ShutdownAntiAddiction()
{
	FOAuthLoginPtr LoginChannel = FOAuthLoginModule::Get().GetOAuthLogin(GOOGLE_CHANNEL_NAME);
	if (LoginChannel.IsValid())
	{
		LoginChannel->OnAntiAddictionEvent.ExecuteIfBound(TEXT("{ \"Event\": " + OAuthLogin::ToString(EAntiAddictionEvent::Shutdown) + ", \"Code\": " + OAuthLogin::ToString(EOAuthResponse::Success) + ", \"Data\": {} }"));
	}
}