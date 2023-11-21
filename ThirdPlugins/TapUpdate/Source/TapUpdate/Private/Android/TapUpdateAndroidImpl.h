#pragma once
#include "TapUpdateImpl.h"

class FTapUpdateAndroidImpl: public FTapUpdateImpl {
public:
	virtual ~FTapUpdateAndroidImpl() override;
	virtual void UpdateGame(FSimpleDelegate OnCancel) override;
};