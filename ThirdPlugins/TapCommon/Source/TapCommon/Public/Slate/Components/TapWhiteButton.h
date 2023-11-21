// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TAPCOMMON_API STapWhiteButton : public SButton
{
public:
	SLATE_BEGIN_ARGS(STapWhiteButton)
		{
		}

	SLATE_ATTRIBUTE( FText, Text )
	
	SLATE_EVENT( FOnClicked, OnClicked )

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
