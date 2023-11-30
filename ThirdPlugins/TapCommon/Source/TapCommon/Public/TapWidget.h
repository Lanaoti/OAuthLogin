// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/EngineVersionComparison.h"
#include "Blueprint/UserWidget.h"
#include "TapWidget.generated.h"


UCLASS()
class TAPCOMMON_API UTapWidget : public UUserWidget
{
	GENERATED_BODY()
public:
#if UE_VERSION_NEWER_THAN(5, 0, 0)
	virtual void AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder);
#else
	virtual void AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder) override;
#endif

	virtual void RemoveFromParent() override;

#if UE_VERSION_NEWER_THAN(5, 0, 0)
	virtual void OnLevelRemovedFromWorld(ULevel* Level, UWorld* World);
#endif
};
