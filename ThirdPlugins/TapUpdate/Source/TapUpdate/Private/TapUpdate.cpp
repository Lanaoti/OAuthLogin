#include "TapUpdate.h"

#include "TapUpdateImpl.h"

void FTapUpdate::UpdateGame(FSimpleDelegate OnCancel) {
	FTapUpdateImpl::Get()->UpdateGame(MoveTemp(OnCancel));
}
