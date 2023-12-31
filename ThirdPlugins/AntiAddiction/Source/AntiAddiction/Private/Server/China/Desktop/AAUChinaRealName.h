#pragma once
#include "TUError.h"
#include "Model/AAURealNameResultModel.h"
#include "TUAuthResult.h"

class AAUChinaRealName {
public:

	// MARK: - 实名认证部分
	static void ManualVerify(const FString& UserID, const FString& Name, const FString& CardID, bool bFallback,
		TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack);

	static void CheckRealNameState(const FString& UserID, TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack);
};
