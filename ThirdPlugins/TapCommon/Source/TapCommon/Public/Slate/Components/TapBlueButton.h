// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TAPCOMMON_API STapBlueButton : public SButton
{
public:
	SLATE_BEGIN_ARGS(STapBlueButton)
		{
		}

	SLATE_ATTRIBUTE( FText, Text )
	
	SLATE_EVENT( FOnClicked, OnClicked )
	
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
