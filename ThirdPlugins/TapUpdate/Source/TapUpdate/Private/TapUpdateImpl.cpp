#include "TapUpdateImpl.h"
#include "TUDebuger.h"
#if PLATFORM_ANDROID
#include "Android/TapUpdateAndroidImpl.h"
#endif

#define UnsupportedPlatformsLog TUDebuger::ErrorLog(FString::Printf(TEXT("Unsupported Platforms Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));

FTapUpdateImpl::~FTapUpdateImpl() {
}

TSharedPtr<FTapUpdateImpl> FTapUpdateImpl::Instance = nullptr;


TSharedPtr<FTapUpdateImpl>& FTapUpdateImpl::Get() {
	if (!Instance.IsValid()) {
#if PLATFORM_ANDROID
		Instance = MakeShared<FTapUpdateAndroidImpl>();
#else
		Instance = MakeShared<FTapUpdateImpl>();
#endif
	}
	return Instance;
}

void FTapUpdateImpl::UpdateGame(FSimpleDelegate OnCancel) {
	UnsupportedPlatformsLog
}
