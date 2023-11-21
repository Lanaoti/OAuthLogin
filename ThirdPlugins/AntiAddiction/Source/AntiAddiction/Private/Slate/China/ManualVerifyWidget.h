// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FAAUChinaAuthIdentifyWord;
class SEditableTextBox;
class AAUImpl;
/**
 * 
 */
class ANTIADDICTION_API SManualVerifyWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SManualVerifyWidget)
		{
		}

	SLATE_EVENT(FSimpleDelegate, OnClose)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, const TSharedRef<AAUImpl>& InImpl, const FAAUChinaAuthIdentifyWord& Word, bool bInFallback);
	
	FReply OnCommitButtonClicked();

	FReply OnCloseButtonClicked();

	TSharedPtr<SEditableTextBox> NameBox;
	TSharedPtr<SEditableTextBox> CardIdBox;

	TSharedPtr<AAUImpl> Impl;
	bool bFallback = true;
	FSimpleDelegate OnClose;
};
