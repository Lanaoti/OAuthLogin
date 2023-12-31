﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"

#include "PermissionCheckBoxWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct TAPLOGIN_API FPermissionCheckBoxStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FPermissionCheckBoxStyle();
	virtual ~FPermissionCheckBoxStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FPermissionCheckBoxStyle& GetDefault();
	
	UPROPERTY(EditAnywhere, Category = "Permission")
	FCheckBoxStyle PermissionCheckBoxStyle;

	UPROPERTY(EditAnywhere, Category = "Permission")
	FTextBlockStyle PermissionTextBlockStyle;
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UPermissionCheckBoxWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FPermissionCheckBoxStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast<const struct FSlateWidgetStyle*>(&WidgetStyle);
	}
};
