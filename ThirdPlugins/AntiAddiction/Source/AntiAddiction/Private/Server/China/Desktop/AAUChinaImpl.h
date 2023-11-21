#pragma once
#include "AAUChinaSever.h"
#include "Model/China/AAUChinaConfigModel.h"
#include "Server/AAUImpl.h"
#include "Server/AAUServerDelegate.h"

struct FTUAccessToken;
struct FTapSmoothError;
class SVerifyEntryWidget;
class SManualVerifyWidget;
struct FAAURealNameResultModel;
struct FTUError;


// class AAUChinaSever;
class AAUChinaImpl: public AAUImpl, AAUServerDelegate {
public:
	AAUChinaImpl();
	virtual ~AAUChinaImpl() override;
	virtual void StartupWithTapTap(const FString& UserId) override;
	virtual void SetTestEnv(bool Enable) override;
	virtual void Exit() override;
	virtual EAAUAgeLimit GetAgeRange() override;
	virtual int GetRemainingTime() override;
	virtual void KickOut(AAUTimeBoundary Boundary, const FString& Title, const FString& Content) override;
	virtual void InitImpl(const FAAUConfig& _Config) override;
	virtual void EnterGame() override;
	virtual void LeaveGame() override;
	virtual void CheckPayLimit(int Amount, TFunction<void(bool Status)> CallBack, TFunction<void(const FString& Msg)> FailureHandler) override;
	virtual void SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack, TFunction<void(const FString& Msg)> FailureHandler) override;
	virtual FString CurrentToken() override;
	
	void TryAgainLogin(const FString& ErrMsg);

	void Login(const FString& AccessToken, EAAUAgeLimit AgeLimit, bool IsFirst = false, bool bTapUser = false);

	void ShowRealNameUI(AAURealNameWordType Type, bool bFallback = true);

	void ShowHealthTipUI(const FString& Title, const FString& Content, int RemainTime, AAUHealthTipType Type);

	void GotoTapTapQuickVerify();

	FString CurrentUserID;
	
protected:
	void StartupQuickVerifyTapAuth();

	void QuickVerify(const TSharedRef<FTUAccessToken>& TapToken);
	
	void HandleQuickVerifyTdsResult(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTapSmoothError& Error);

	TWeakPtr<SVerifyEntryWidget> EntryWidget;
	TWeakPtr<SManualVerifyWidget> ManualVerifyWidget;
	TSharedPtr<AAUChinaSever> Server;
};
