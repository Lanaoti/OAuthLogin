// Copyright 2023 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"

DECLARE_DELEGATE_OneParam(FOnInitCompleted, const FString&);
DECLARE_DELEGATE_OneParam(FOnLoginCompleted, const FString&);
DECLARE_DELEGATE_OneParam(FOnLogoutCompleted, const FString&);
DECLARE_DELEGATE_OneParam(FOnAntiAddictionEvent, const FString&);

class OAUTHLOGIN_API FGenericOAuthLogin : public TSharedFromThis<FGenericOAuthLogin>
{
public:
	FGenericOAuthLogin();
	virtual ~FGenericOAuthLogin();

	// 初始化
	virtual void Init() = 0;

	// 登录
	virtual void Login() = 0;

	// 登出
	virtual void Logout() = 0;

	// 启动防沉迷
	virtual void StartupAntiAddiction() = 0;

	// 关闭防沉迷
	virtual void ShutdownAntiAddiction() = 0;

	// 初始化完成回调
	FOnInitCompleted OnInitCompleted;

	// 登录完成回调
	FOnLoginCompleted OnLoginCompleted;

	// 注销完成回调
	FOnLogoutCompleted OnLogoutCompleted;

	// 防沉迷事件回调
	FOnAntiAddictionEvent OnAntiAddictionEvent;
};
