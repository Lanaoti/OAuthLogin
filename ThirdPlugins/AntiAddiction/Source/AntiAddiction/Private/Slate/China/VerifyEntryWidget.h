// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FAAUChinaAuthIdentifyWord;
class AAUImpl;
class STapBoxWrapper;
/**
 * 
 */
class ANTIADDICTION_API SVerifyEntryWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SVerifyEntryWidget)
		{
		}

	SLATE_EVENT(FSimpleDelegate, OnManualVerifyClicked);

	SLATE_EVENT(FSimpleDelegate, OnTapQuickVerifyClicked);
	
	SLATE_EVENT(FSimpleDelegate, OnClose)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<AAUImpl>& InImpl, const FAAUChinaAuthIdentifyWord& Word);

protected:
	FReply OnManualVerifyButtonClicked();

	FReply OnTapQuickVerifyButtonClicked();

	FReply OnCloseButtonClicked();

	TSharedPtr<AAUImpl> Impl;
	
	FSimpleDelegate OnClose;

private:
	TSharedPtr<STapBoxWrapper> Wrapper;
};
