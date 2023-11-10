// Copyright 2022 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OAuthLoginTypes.h"
#include "OAuthLoginBlueprintLibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOAuthLoginDelegate, EOAuthLoginResponse, Code, const FOAuthData&, OAuthData);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOAuthLogoutDelegate, EOAuthLoginResponse, Code);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FAntiAddictionEventDelegate, EAntiAddictionEvent, Event, EOAuthLoginResponse, Code, const FAntiAddictionData&, AntiAddictionData);

UCLASS()
class OAUTHLOGIN_API UOAuthLoginBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = OAuthLogin)
	static void Init();

	UFUNCTION(BlueprintCallable, Category = OAuthLogin)
	static bool IsEnabled(FName ChannelName);

	UFUNCTION(BlueprintCallable, Category = OAuthLogin)
	static void Login(FName ChannelName, FOAuthLoginDelegate Delegate);

	UFUNCTION(BlueprintCallable, Category = OAuthLogin)
	static void Logout(FName ChannelName, FOAuthLogoutDelegate Delegate);

	UFUNCTION(BlueprintCallable, Category = OAuthLogin)
	static void StartupAntiAddiction(FName ChannelName, FAntiAddictionEventDelegate Delegate);

	UFUNCTION(BlueprintCallable, Category = OAuthLogin)
	static void ShutdownAntiAddiction(FName ChannelName);
};
