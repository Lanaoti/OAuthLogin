// Copyright 2023 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "OAuthLoginTypes.h"
#include "GenericOAuthLogin.h"

DECLARE_LOG_CATEGORY_EXTERN(LogOAuthLogin, Log, All);

DECLARE_DELEGATE_ThreeParams(FOnInitEvent, FName /*ChannelName*/, EOAuthResponse /*Code*/, bool /*bWasCompleted*/);

typedef TSharedPtr<FGenericOAuthLogin> FOAuthLoginPtr;
typedef TSharedRef<FGenericOAuthLogin> FOAuthLoginRef;

class FOAuthLoginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	OAUTHLOGIN_API static inline FOAuthLoginModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FOAuthLoginModule>("OAuthLogin");
	}

	OAUTHLOGIN_API static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("OAuthLogin");
	}

	OAUTHLOGIN_API FOAuthLoginPtr GetOAuthLogin(FName ChannelName);

	OAUTHLOGIN_API void Register(FName ChannelName, FOAuthLoginRef LoginChannel);
	OAUTHLOGIN_API void Unregister(FName ChannelName);

	void Init();
	bool IsEnabled(FName ChannelName) const;

public:
	FOnInitEvent OnInitEvent;

private:
	static TMap<FName, FOAuthLoginRef> LoginChannels;

	int32 Count;

	int32 Total;
};
