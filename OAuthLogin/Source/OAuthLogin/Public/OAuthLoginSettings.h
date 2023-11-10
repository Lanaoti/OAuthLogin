// Copyright 2023 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OAuthLoginSettings.generated.h"


/**
 * UOAuthLoginSettings
 */
UCLASS(Config=Game)
class OAUTHLOGIN_API UOAuthLoginSettings : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = Build)
	bool bEnableHuaWei;

	UPROPERTY(Config, EditAnywhere, Category = Build)
	bool bEnableWeChat;

	UPROPERTY(Config, EditAnywhere, Category = Build)
	bool bEnableQQ;

	UPROPERTY(Config, EditAnywhere, Category = Build)
	bool bEnableTapTap;

	UPROPERTY(Config, EditAnywhere, Category = Build)
	bool bEnableHaoYouKuaiBao;

	UPROPERTY(Config, EditAnywhere, Category = Build)
	bool bEnableGoogle;

	UPROPERTY(Config, EditAnywhere, Category = Build)
	bool bEnableFacebook;

	UPROPERTY(Config, EditAnywhere, Category = Build)
	bool bEnableSteam;
};