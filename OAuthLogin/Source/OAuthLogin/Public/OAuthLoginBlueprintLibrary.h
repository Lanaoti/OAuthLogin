// Copyright 2022 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OAuthLoginTypes.h"
#include "OAuthLoginBlueprintLibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOAuthInitDelegate, FName, ChannelName, EOAuthResponse, Code, bool, bWasCompleted);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOAuthLoginDelegate, FName, ChannelName, EOAuthResponse, Code, const FOAuthData&, Data);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOAuthLogoutDelegate, FName, ChannelName, EOAuthResponse, Code);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FAntiAddictionEventDelegate, EAntiAddictionEvent, Event, EOAuthResponse, Code, const FAntiAddictionData&, Data);

UCLASS()
class OAUTHLOGIN_API UOAuthLoginBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = OAuthLogin)
	static void Init(FOAuthInitDelegate Delegate);

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
