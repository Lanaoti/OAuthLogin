// Copyright 2022 CQUnreal. All Rights Reserved.

#include "AndroidHuaWei.h"
#include "Android/AndroidJavaEnv.h"
#include "HuaWeiModule.h"

jmethodID FAndroidHuaWei::HuaWeiInit;
jmethodID FAndroidHuaWei::HuaWeiLogin;
jmethodID FAndroidHuaWei::HuaWeiSilentLogin;

//登录回调
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeHuaWeiLoginResultNotify(JNIEnv* jenv, jobject thiz, jstring msg)
{
	FString ReturnString = TEXT("");
	if (jenv != nullptr)
	{
		ReturnString = FJavaHelper::FStringFromParam(jenv, msg);
	}

	FHuaWeiModule& HuaWeiModule = FHuaWeiModule::Get();
	if (!HuaWeiModule.IsAvailable())
	{
		return;
	}

	FHuaWeiPtr HuaWei = HuaWeiModule.GetHuaWei();
	if (!HuaWei.IsValid())
	{
		return;
	}

	HuaWei->HuaWeiLoginCompleted.Broadcast(ReturnString);
}

//静默登录回调
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeHuaWeiSilentLoginResultNotify(JNIEnv* jenv, jobject thiz, jstring msg)
{
	FString ReturnString = TEXT("");
	if (jenv != nullptr)
	{
		ReturnString = FJavaHelper::FStringFromParam(jenv, msg);
	}

	FHuaWeiModule& HuaWeiModule = FHuaWeiModule::Get();
	if (!HuaWeiModule.IsAvailable())
	{
		return;
	}

	FHuaWeiPtr HuaWei = HuaWeiModule.GetHuaWei();
	if (!HuaWei.IsValid())
	{
		return;
	}

	HuaWei->HuaWeiSilentLoginCompleted.Broadcast(ReturnString);
}



//防沉迷回调玩家当前无法进行游戏
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeHuaWeiAntiAddictionNotify(JNIEnv* jenv, jobject thiz, jstring msg)
{
	FString ReturnString = TEXT("");
	if (jenv != nullptr)
	{
		ReturnString = FJavaHelper::FStringFromParam(jenv, msg);
	}

	FHuaWeiModule& HuaWeiModule = FHuaWeiModule::Get();
	if (!HuaWeiModule.IsAvailable())
	{
		return;
	}

	FHuaWeiPtr HuaWei = HuaWeiModule.GetHuaWei();
	if (!HuaWei.IsValid())
	{
		return;
	}

	HuaWei->HuaWeiAntiAddictionCompleted.Broadcast(ReturnString);
}


//华为初始化回调
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_nativeHuaWeiInitResultNotify(JNIEnv* jenv, jobject thiz, jstring msg)
{
	FString ReturnString = TEXT("");
	if (jenv != nullptr)
	{
		ReturnString = FJavaHelper::FStringFromParam(jenv, msg);
	}

	FHuaWeiModule& HuaWeiModule = FHuaWeiModule::Get();
	if (!HuaWeiModule.IsAvailable())
	{
		return;
	}

	FHuaWeiPtr HuaWei = HuaWeiModule.GetHuaWei();
	if (!HuaWei.IsValid())
	{
		return;
	}

	HuaWei->HuaWeiInitCompleted.Broadcast(ReturnString);
}

FAndroidHuaWei::FAndroidHuaWei()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		HuaWeiInit = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_HuaWei_Init", "()V", false);
		HuaWeiLogin = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_HuaWei_Login", "()V", false);
		HuaWeiSilentLogin = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_HuaWei_silentLogin", "()V", false);
	}
}

FAndroidHuaWei::~FAndroidHuaWei()
{

}

void FAndroidHuaWei::OnStartup()
{

}

void FAndroidHuaWei::OnShutdown()
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

void FAndroidHuaWei::SilentLogin()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaWeiSilentLogin);
	}
}