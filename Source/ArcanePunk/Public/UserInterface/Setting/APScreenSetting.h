// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APScreenSetting.generated.h"

class USlider;
class UTextBlock;
class UButton;
class UImage;
class UAPGameInstance;

UCLASS()
class ARCANEPUNK_API UAPScreenSetting : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnGammaSlide(float Value);
	UFUNCTION()
	void OnSaturationSlide(float Value);
	UFUNCTION()
	void OnContrastSlide(float Value);
	UFUNCTION()
	void OnGainSlide(float Value);
	UFUNCTION()
	void OnOffsetSlide(float Value);

private:
	void BindButtonAndSlider();
	void InitSlider();
	void InitUIMaterials();

	void SetSaturationUIMaterials(float Value);
	void SetContrastUIMaterials(float Value);

	UFUNCTION()
	void OnApply();
	UFUNCTION()
	void OnCancel();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* GammaSlider;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_GammaNum;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* SaturationSlider;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_SaturationNum;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* ContrastSlider;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_ContrastNum;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* GainSlider;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_GainNum;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* OffsetSlider;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_OffsetNum;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Apply_Button;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RollBack_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Image_WhiteBG;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Image_WhiteSymbol;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Image_BlackBG;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Image_BlackSymbol;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Image_ColorBG;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Image_ColorSymbol;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextBlock_Saturation;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextBlock_Contrast;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextBlock_Gamma;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextBlock_Gain;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextBlock_Offset;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextBlock_Init;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextBlock_Apply;

	float Saturation = 0.5f;
	float Contrast = 0.5f;
	float Gamma = 0.5f;
	float Gain = 0.5f;
	float Offset = 0.5f;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> UIMaterials;

	TWeakObjectPtr<UAPGameInstance> APGI;
};
