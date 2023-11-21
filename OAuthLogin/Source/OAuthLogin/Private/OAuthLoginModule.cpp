// Copyright 2023 CQUnreal. All Rights Reserved.

#include "OAuthLoginModule.h"
#include "JsonUtilities.h"

/** Logging definitions */
DEFINE_LOG_CATEGORY(LogOAuthLogin);

#define LOCTEXT_NAMESPACE "FOAuthLoginModule"

TMap<FName, FOAuthLoginRef> FOAuthLoginModule::LoginChannels;

void FOAuthLoginModule::StartupModule()
{
#if ENABLE_HUAWEI
	FModuleManager::Get().LoadModuleChecked(TEXT("HuaWei"));
#endif

#if ENABLE_WECHAT
	FModuleManager::Get().LoadModuleChecked(TEXT("WeChat"));
#endif

#if ENABLE_QQ
	FModuleManager::Get().LoadModuleChecked(TEXT("QQ"));
#endif

#if ENABLE_TAPTAP
	FModuleManager::Get().LoadModuleChecked(TEXT("TapTap"));
#endif

#if ENABLE_HAOYOUKUAIBAO
	FModuleManager::Get().LoadModuleChecked(TEXT("HaoYouKuaiBao"));
#endif

#if ENABLE_GOOGLE
	FModuleManager::Get().LoadModuleChecked(TEXT("Google"));
#endif

#if ENABLE_FACEBOOK
	FModuleManager::Get().LoadModuleChecked(TEXT("Facebook"));
#endif

#if ENABLE_STEAM
	FModuleManager::Get().LoadModuleChecked(TEXT("Steam"));
#endif

	Count = 0;

	Total = LoginChannels.Num();
}

void FOAuthLoginModule::ShutdownModule()
{
#if ENABLE_HUAWEI
	FModuleManager::Get().UnloadModule(TEXT("HuaWei"), true);
#endif

#if ENABLE_WECHAT
	FModuleManager::Get().UnloadModule(TEXT("WeChat"), true);
#endif

#if ENABLE_QQ
	FModuleManager::Get().UnloadModule(TEXT("QQ"), true);
#endif

#if ENABLE_TAPTAP
	FModuleManager::Get().UnloadModule(TEXT("TapTap"), true);
#endif

#if ENABLE_HAOYOUKUAIBAO
	FModuleManager::Get().UnloadModule(TEXT("HaoYouKuaiBao"), true);
#endif

#if ENABLE_GOOGLE
	FModuleManager::Get().UnloadModule(TEXT("Google"), true);
#endif

#if ENABLE_FACEBOOK
	FModuleManager::Get().LoadModuleChecked(TEXT("Facebook"), true);
#endif

#if ENABLE_STEAM
	FModuleManager::Get().LoadModuleChecked(TEXT("Steam"), true);
#endif
}

FOAuthLoginPtr FOAuthLoginModule::GetOAuthLogin(FName ModuleName)
{
	FOAuthLoginRef* OAuthLogin = LoginChannels.Find(ModuleName);
	if (OAuthLogin != nullptr)
	{
		return *OAuthLogin;
	}

	return nullptr;
}

void FOAuthLoginModule::Register(FName ChannelName, FOAuthLoginRef LoginChannel)
{
	LoginChannels.Add(ChannelName, LoginChannel);
}

void FOAuthLoginModule::Unregister(FName ChannelName)
{
	LoginChannels.Remove(ChannelName);
}

void FOAuthLoginModule::Init()
{
	if (Total <= 0 || Count >= Total)
	{
		UE_LOG(LogOAuthLogin, Warning, TEXT("OAuthLogin completed, no channels were activated at the time"));

		OnInitEvent.ExecuteIfBound("", EOAuthResponse::Success, true);
		return;
	}

	for (TPair<FName, FOAuthLoginRef> Pair : LoginChannels)
	{
		UE_LOG(LogOAuthLogin, Log, TEXT("Starting init %s channel"), *Pair.Key.ToString());

		Pair.Value->OnInitCompleted = FOnInitCompleted::CreateLambda([this, Pair](const FString& Data) {
			bool bWasCompleted = (++Count >= Total);

			FOAuthInitData OAuthInitData;
			FJsonObjectConverter::JsonObjectStringToUStruct<FOAuthInitData>(Data, &OAuthInitData);

			UE_LOG(LogOAuthLogin, Log, TEXT("On init completed %s Count: %d Total: %d"), *Pair.Key.ToString(), Count, Total);

			OnInitEvent.ExecuteIfBound(Pair.Key, OAuthInitData.Code, bWasCompleted);
		});
		Pair.Value->Init();
	}
}

bool FOAuthLoginModule::IsEnabled(FName ChannelName) const
{
	return LoginChannels.Contains(ChannelName);
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FOAuthLoginModule, OAuthLogin)