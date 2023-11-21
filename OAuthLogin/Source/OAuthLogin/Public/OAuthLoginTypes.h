// Copyright 2023 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OAuthLoginTypes.generated.h"


UENUM(BlueprintType)
enum class EOAuthLoginType : uint8
{
	HuaWei,						// HuaWei
	TapTap,						// TapTap
	WeChat,						// 微信
	QQ,							// QQ
	HaoYouKuaiBao,				// 好友快爆
	Google,						// Google
	Facebook,					// Facebook
};

UENUM(BlueprintType)
enum class EOAuthResponse : uint8
{
	Success,				// 成功
	Canceled,				// 取消
	NotSupported,			// 不支持
	SystemError,			// 系统错误
};

USTRUCT(BlueprintType)
struct FOAuthInitData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	EOAuthResponse Code;
};

USTRUCT(BlueprintType)
struct FOAuthData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	FString UID;

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	FString Nickname;

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	FString Avatar;

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	FString Key;

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	FString AccessToken;
};

USTRUCT(BlueprintType)
struct FOAuthLoginData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	EOAuthResponse Code;

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	FOAuthData Data;
};

USTRUCT(BlueprintType)
struct FOAuthLogoutData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	EOAuthResponse Code;
};

UENUM(BlueprintType)
enum class EAntiAddictionEvent : uint8
{
	Startup,					// 启动
	Shutdown,					// 关闭
};

USTRUCT(BlueprintType)
struct FAntiAddictionData
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FAntiAddictionEventData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	EAntiAddictionEvent Event;

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	EOAuthResponse Code;

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	FAntiAddictionData Data;
};

namespace OAuthLogin
{
	template<typename T> FString ToString(T Value)
	{
		return FString::Printf(TEXT("%d"), (int32)Value);
	}
};