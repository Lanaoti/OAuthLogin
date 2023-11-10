// Copyright 2023 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "OAuthLoginTypes.h"
#include "GenericOAuthLogin.h"

DECLARE_LOG_CATEGORY_EXTERN(LogOAuthLogin, Log, All);

typedef TSharedPtr<FGenericOAuthLogin> FOAuthLoginPtr;
typedef TSharedRef<FGenericOAuthLogin> FOAuthLoginRef;

class OAUTHLOGIN_API FOAuthLoginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static inline FOAuthLoginModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FOAuthLoginModule>("OAuthLogin");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("OAuthLogin");
	}

	FOAuthLoginPtr GetOAuthLogin(FName ChannelName);

	void Init();
	bool IsEnabled(FName ChannelName) const;
	void Register(FName ChannelName, FOAuthLoginRef LoginChannel);
	void Unregister(FName ChannelName);

private:
	static TMap<FName, FOAuthLoginRef> LoginChannels;
};
