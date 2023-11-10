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
enum class EOAuthLoginResponse : uint8
{
	Success,				// 成功
	Canceled,				// 取消
	NotSupported,			// 不支持
	SystemError,			// 系统错误
};

USTRUCT(BlueprintType)
struct FOAuthData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	FString UID;

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	FString Nickname;

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	FString Avator;

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	FString Key;

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	FString AccessToken;
};

USTRUCT(BlueprintType)
struct FOAuthLoginData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	EOAuthLoginResponse Code;

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	FOAuthData OAuthData;
};

USTRUCT(BlueprintType)
struct FOAuthLogoutData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	EOAuthLoginResponse Code;
};

UENUM(BlueprintType)
enum class EAntiAddictionEvent : uint8
{
	Init,					// 初始化
};

USTRUCT(BlueprintType)
struct FAntiAddictionData
{
	GENERATED_USTRUCT_BODY()
};

USTRUCT(BlueprintType)
struct FAntiAddictionEventData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	EAntiAddictionEvent Event;

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	EOAuthLoginResponse Code;

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	FAntiAddictionData AntiAddictionData;
};
