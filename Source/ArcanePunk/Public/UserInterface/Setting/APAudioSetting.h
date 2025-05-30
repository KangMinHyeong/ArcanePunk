// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APAudioSetting.generated.h"

class UButton;
class USlider;
class UAPGameInstance;
class UTextBlock;
class UAPSoundSubsystem;

UCLASS()
class ARCANEPUNK_API UAPAudioSetting : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
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
	UFUNCTION()
	void OnSlide_UI(float Value);

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
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	USlider* Slider_UI;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_Master;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_BGM;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_Effect;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_UI;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_Init;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_Apply;

	float Master;
	float BGM;
	float Effect;
	float UI;

	TWeakObjectPtr<UAPSoundSubsystem> SoundGI;
};
