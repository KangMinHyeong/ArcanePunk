// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APAudioSetting.generated.h"

class UButton;
class USlider;

UCLASS()
class ARCANEPUNK_API UAPAudioSetting : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseWheel( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)override;

	void InitSliders();
	void BindButton();

	UFUNCTION()
	void OnClickBack();
	UFUNCTION()
	void OnClickApply();

	UFUNCTION()
	void OnSlide_Master(float Value);
	UFUNCTION()
	void OnSlide_BGM(float Value);
	UFUNCTION()
	void OnSlide_Effect(float Value);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Back;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Apply;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	USlider* Slider_Master;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	USlider* Slider_BGM;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	USlider* Slider_Effect;

	float Master;
	float BGM;
	float Effect;

};
