// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class TUWebAuthHelper;
struct FTUAccessToken;
struct FTUQrCodeModel;
class SQrCodeScanTipButton;
enum class ERegionType : uint8;
class SImage;
class TUAuthResult;

DECLARE_DELEGATE_OneParam(FTapAuthComplete, const TUAuthResult& Result);
/**
 * 
 */
class TAPLOGIN_API STapLoginWidget : public SCompoundWidget, public FGCObject
{
public:
	STapLoginWidget();
		
	SLATE_BEGIN_ARGS(STapLoginWidget)
		{
		}

	SLATE_ARGUMENT_DEFAULT(bool, bRequestProfile){true};

	SLATE_ARGUMENT_DEFAULT(bool, bShowPermissionBox){false};
	
	SLATE_EVENT(FTapAuthComplete, OnAuthComplete);

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, const TArray<FString>& InPermissions, ERegionType Region);

	void RefreshQrCode();

protected:
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	// button start
	FReply OnCloseButtonClicked();

	FReply OnWebAuthButtonClicked();

	FReply OnQrCodeRefreshClicked();
	
	
	void Close(const TUAuthResult& Result);

	void GetProfile(const TSharedPtr<FTUAccessToken>& AccessToken);

	void GetTokenFromWebCode(const FString& WebCode);
	

	// qr-code tip start
	void OnRequestScanTipPanel(const FString& Url, TSharedRef<SQrCodeScanTipButton> FireButton);
	
	void OnRequestCloseScanTipPanel();

	void UpdateScanTipBrush(UTexture2D* ImageTexture, bool bAndroid);
	
	// qr-code start
	void ShowQrCodeRetryButton(bool bShow);
	
	void RefreshQrCodeBrush();
	
	void StartCheckQrCode();
	
	void StopCheckQrCode();

	bool TimerCheckQrCode(float DeltaSecond);

	TSharedPtr<SImage> QrCodeImage;

	TSharedPtr<SOverlay> RetryOverlay;
	
	TSharedPtr<SQrCodeScanTipButton> IOSButton;
	TSharedPtr<SQrCodeScanTipButton> AndroidButton;

	TSharedPtr<SBorder> TipBorder;
	TSharedPtr<SImage> TipImage;

	FSlateBrush ScanTipBrush;
	UTexture2D* ScanTipTextureIOS = nullptr;
	UTexture2D* ScanTipTextureAndroid = nullptr;
	
	FSlateBrush QrCodeBrush;
	UTexture2D* QrCodeTexture = nullptr;
	
	TArray<FString> Permissions;

	FDelegateHandle CheckQcCodeTimer;
	TSharedPtr<FTUQrCodeModel> QrCodeModel;

	TSharedPtr<TUWebAuthHelper> WebAuthHelper;

	bool bRequestProfile = false;
	FTapAuthComplete OnTapAuthComplete;

	FString ScanState;
	
	TMap<FString, FText> PermissionDisplaySettings;

	SOverlay::FOverlaySlot* RetrySlot = nullptr;
	SVerticalBox::FSlot* NoPermissionSlot = nullptr;
	SVerticalBox::FSlot* PermissionSlot = nullptr;
};
