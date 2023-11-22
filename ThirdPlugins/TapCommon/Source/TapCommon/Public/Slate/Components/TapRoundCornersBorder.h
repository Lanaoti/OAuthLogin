// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Layout/SBorder.h"

/**
 * 
 */
class TAPCOMMON_API STapRoundCornersBorder : public SBorder, public FGCObject
{
public:
	virtual FString GetReferencerName() const { return TEXT("STapRoundCornersBorder"); }

	SLATE_BEGIN_ARGS(STapRoundCornersBorder)
		{
		}

	SLATE_ARGUMENT_DEFAULT(bool, bHalfHeightRadius){false};
	
	SLATE_ARGUMENT_DEFAULT(float, CornerRadius){8.f};
	
	SLATE_ARGUMENT_DEFAULT(UTexture2D*, IconTexture){nullptr};
	
	SLATE_DEFAULT_SLOT( FArguments, Content )

	SLATE_ARGUMENT( EHorizontalAlignment, HAlign )
	SLATE_ARGUMENT( EVerticalAlignment, VAlign )
	SLATE_ATTRIBUTE( FMargin, Padding )
	SLATE_ATTRIBUTE( FSlateColor, BorderBackgroundColor )

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	TSharedPtr<FSlateBrush> Brush;

	UMaterialInterface* GCMat = nullptr;
};
