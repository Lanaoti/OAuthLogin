#pragma once
#include "TUCommonImpl.h"

class TUCommonMobileImpl: public TUCommonImpl {
public:
	TUCommonMobileImpl();
	virtual void SetLanguage(ELanguageType LanguageType) override;
	virtual void setDurationStatisticsEnabled(bool bEnable) override;
};
