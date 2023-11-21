#pragma once
#include "TapUpdate.h"

// class FTapUpdateImpl {
// public:
// 	virtual void UpdateGame(FTapUpdate::FCancelDelegate OnCancel);
// };


class FTapUpdateImpl {
public:
	virtual ~FTapUpdateImpl();
	static TSharedPtr<FTapUpdateImpl>& Get();
	
	virtual void UpdateGame(FSimpleDelegate OnCancel);

private:
	static TSharedPtr<FTapUpdateImpl> Instance;
	
};
