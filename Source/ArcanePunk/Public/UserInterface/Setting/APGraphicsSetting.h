// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APGraphicsSetting.generated.h"

class UButton;
class UComboBoxString;
class UTextBlock;

UENUM(BlueprintType)
enum class EGraphicsSetting : uint8 // 그래픽 설정
{
	Low 	= 0,
	Middle 	= 1,
	High 	= 2,
	Epic 	= 3
};

UCLASS()
class ARCANEPUNK_API UAPGraphicsSetting : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseWheel( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)override;
	
	void InitGraphicsSetting();
	void InitWindowSetting();
	void InitBindSetting();

private:
	void BindComboBox();
	void BindButton();

	void SetRS_Setting();
	void SetFrameRate_Setting();

	UFUNCTION()
	void OnSelectChange_WindowMode(FString Items, ESelectInfo::Type type);

	UFUNCTION()
	void OnSelectChange_ScreenResolution(FString Items, ESelectInfo::Type type);

	UFUNCTION()
	void OnClickButton_45fps();
	UFUNCTION()
	void OnClickButton_60fps();
	UFUNCTION()
	void OnClickButton_120fps();
	UFUNCTION()
	void OnClickButton_Unlimited();
	void ChangeFrameRate(float fps);

	UFUNCTION()
	void OnClickOS_Low();
	UFUNCTION()
	void OnClickOS_Middle();
	UFUNCTION()
	void OnClickOS_High();
	UFUNCTION()
	void OnClickOS_Epic();
	void Change_OS();

	UFUNCTION()
	void OnClickPP_Low();
	UFUNCTION()
	void OnClickPP_Middle();
	UFUNCTION()
	void OnClickPP_High();
	UFUNCTION()
	void OnClickPP_Epic();
	void Change_PP(int32 PP);

	UFUNCTION()
	void OnClickTQ_Low();
	UFUNCTION()
	void OnClickTQ_Middle();
	UFUNCTION()
	void OnClickTQ_High();
	UFUNCTION()
	void OnClickTQ_Epic();
	void Change_TQ(int32 TQ);

	UFUNCTION()
	void OnClickAA_Low();
	UFUNCTION()
	void OnClickAA_Middle();
	UFUNCTION()
	void OnClickAA_High();
	UFUNCTION()
	void OnClickAA_Epic();
	void Change_AA(int32 AA);

	UFUNCTION()
	void OnClickSQ_Low();
	UFUNCTION()
	void OnClickSQ_Middle();
	UFUNCTION()
	void OnClickSQ_High();
	UFUNCTION()
	void OnClickSQ_Epic();
	void Change_SQ(int32 SQ);

	UFUNCTION()
	void OnClickEQ_Low();
	UFUNCTION()
	void OnClickEQ_Middle();
	UFUNCTION()
	void OnClickEQ_High();
	UFUNCTION()
	void OnClickEQ_Epic();
	void Change_EQ(int32 EQ);

	UFUNCTION()
	void OnClickFQ_Low();
	UFUNCTION()
	void OnClickFQ_Middle();
	UFUNCTION()
	void OnClickFQ_High();
	UFUNCTION()
	void OnClickFQ_Epic();
	void Change_FQ(int32 FQ);

	UFUNCTION()
	void OnClickShading_Low();
	UFUNCTION()
	void OnClickShading_Middle();
	UFUNCTION()
	void OnClickShading_High();
	UFUNCTION()
	void OnClickShading_Epic();
	void Change_Shading(int32 Shading);
	
	UFUNCTION()
	void OnClickRQ_Low();
	UFUNCTION()
	void OnClickRQ_Middle();
	UFUNCTION()
	void OnClickRQ_High();
	UFUNCTION()
	void OnClickRQ_Epic();
	void Change_RQ(int32 RQ);

	UFUNCTION()
	void OnClickVD_Low();
	UFUNCTION()
	void OnClickVD_Middle();
	UFUNCTION()
	void OnClickVD_High();
	UFUNCTION()
	void OnClickVD_Epic();
	void Change_VD(int32 VD);

	UFUNCTION()
	void OnClickGI_Low();
	UFUNCTION()
	void OnClickGI_Middle();
	UFUNCTION()
	void OnClickGI_High();
	UFUNCTION()
	void OnClickGI_Epic();
	void Change_GI(int32 GI);

	UFUNCTION()
	void OnClickRS_Low();
	UFUNCTION()
	void OnClickRS_Middle();
	UFUNCTION()
	void OnClickRS_High();
	UFUNCTION()
	void OnClickRS_Epic();
	void Change_RS(float RS);

	UFUNCTION()
	void OnClickButton_Back();
	UFUNCTION()
	void OnClickButton_Save();

	UFUNCTION()
	void OnClickButton_OptimalSetting();

private:
//  Window Mode Select
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UComboBoxString* WindowMode;

//  Screen Resolution Size
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UComboBoxString* ScreenResolution;

//  Frame Rate Limit
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button_45fps;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_60fps;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_120fps;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Unlimited;

	EGraphicsSetting FrameRateSetting;

//  OverAll Scalability
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* OS_Low;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* OS_Middle;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* OS_High;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* OS_Epic;

	EGraphicsSetting OS_Setting;

//  Post Process
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PP_Low;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* PP_Middle;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* PP_High;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* PP_Epic;

	EGraphicsSetting PP_Setting;

//  Texture Quality
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* TQ_Low;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* TQ_Middle;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* TQ_High;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* TQ_Epic;

	EGraphicsSetting TQ_Setting;

//  Anti Aliasing
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* AA_Low;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* AA_Middle;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* AA_High;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* AA_Epic;

	EGraphicsSetting AA_Setting;

//  Shadow Quality
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SQ_Low;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* SQ_Middle;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* SQ_High;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* SQ_Epic;

	EGraphicsSetting SQ_Setting;

//  Effect Quality
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* EQ_Low;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* EQ_Middle;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* EQ_High;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* EQ_Epic;

	EGraphicsSetting EQ_Setting;

//  Foliage Quality
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* FQ_Low;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* FQ_Middle;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* FQ_High;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* FQ_Epic;

	EGraphicsSetting FQ_Setting;

//  Shading Quality
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Shading_Low;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Shading_Middle;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Shading_High;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Shading_Epic;

	EGraphicsSetting Shading_Setting;

//	Reflection Quality
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* RQ_Low;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* RQ_Middle;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* RQ_High;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* RQ_Epic;

	EGraphicsSetting RQ_Setting;

//	View Distance
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* VD_Low;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* VD_Middle;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* VD_High;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* VD_Epic;

	EGraphicsSetting VD_Setting;

//  Global Illumination Quality
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* GI_Low;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* GI_Middle;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* GI_High;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* GI_Epic;

	EGraphicsSetting GI_Setting;

//  Resolution Scale 
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* RS_Low;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* RS_Middle;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* RS_High;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* RS_Epic;

	EGraphicsSetting RS_Setting;	

//  Back to the Option Setting
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Back;

//  Svae Graphics Setting
	// UPROPERTY(EditAnywhere, meta = (BindWidget))	
	// UButton* Button_Save;

//	Optimal Setting
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_OptimalSetting;

	UPROPERTY(EditAnywhere)
	FLinearColor SelectedButtonColor;

	UPROPERTY(EditAnywhere)
	FLinearColor DefaultButtonColor;
//

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_Window;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_AspectRatio;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_MaxFPS;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_OverAll;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Texture;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_AntiAliasing;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_PostProcessing;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Shadow;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Effect;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Foliage;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Shading;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Reflection;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_ViewDistance;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_GlobalIllumination;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_ResolutionScale;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_NoLimit;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_Init;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_Optimization;

	TArray<UButton*> Btn_Low;
	TArray<UButton*> Btn_Middle;
	TArray<UButton*> Btn_High;
	TArray<UButton*> Btn_Epic;
};
