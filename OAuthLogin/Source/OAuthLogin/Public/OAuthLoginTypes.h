// Copyright 2023 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
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

	FOAuthInitData()
		: Code(EOAuthResponse::Success)
	{

	}
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
	FString AuthToken;

	FOAuthData()
		: UID(TEXT(""))
		, Nickname(TEXT(""))
		, Avatar(TEXT(""))
		, AuthToken(TEXT(""))
	{

	}
};

USTRUCT(BlueprintType)
struct FOAuthLoginData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	EOAuthResponse Code;

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	FOAuthData Data;

	FOAuthLoginData()
		: Code(EOAuthResponse::Success)
	{

	}
};

USTRUCT(BlueprintType)
struct FOAuthLogoutData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = OAuthLogin)
	EOAuthResponse Code;

	FOAuthLogoutData()
		: Code(EOAuthResponse::Success)
	{

	}
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

	FAntiAddictionData()
	{

	}
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

	FAntiAddictionEventData()
		: Event(EAntiAddictionEvent::Startup)
		, Code(EOAuthResponse::Success)
	{

	}
};

namespace OAuthLogin
{
	template<typename T> FString ToString(T Value)
	{
		return FString::Printf(TEXT("%d"), (int32)Value);
	}

	template<typename T> FString PraseData(const T& Data)
	{
		FString Result = "{}";

		FJsonObjectConverter::UStructToJsonObjectString(T::StaticStruct(), &Data, Result);
		
		return Result;
	}
};