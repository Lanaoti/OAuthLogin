// Copyright 2023 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OAuthLoginTypes.generated.h"


UENUM(BlueprintType)
enum class EOAuthLoginType : uint8
{
	HuaWei,						// HuaWei
	TapTap,						// TapTap
	WeChat,						// ΢��
	QQ,							// QQ
	HaoYouKuaiBao,				// ���ѿ챬
	Google,						// Google
	Facebook,					// Facebook
};

UENUM(BlueprintType)
enum class EOAuthLoginResponse : uint8
{
	Success,				// �ɹ�
	Canceled,				// ȡ��
	NotSupported,			// ��֧��
	SystemError,			// ϵͳ����
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
	Init,					// ��ʼ��
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
