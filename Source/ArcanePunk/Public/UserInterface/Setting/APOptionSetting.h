// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APOptionSetting.generated.h"

class UButton;
class UAPGraphicsSetting;
class UAPAudioSetting;

UCLASS()
class ARCANEPUNK_API UAPOptionSetting : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseWheel( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

	void BindButton();

	UFUNCTION()
	void OnClickButton_Graphics();
	UFUNCTION()
	void OnClickButton_Audio();
	UFUNCTION()
	void OnClickButton_Key();

	UFUNCTION()
	void OnClickButton_Back();

	// Option Setting
	void InitGraphicsSetting();
	void InitAudioSetting();

	void SmartKeySetting();
	void GraphicsSetting();
	void AudioSetting();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button_Graphics;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Audio;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Key;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Back;


	// Option Setting 
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GraphicsSettingClass; // Graphics

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> AudioSettingClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SmartKeySettingClass;

	TWeakObjectPtr<UAPGraphicsSetting> GraphicsSettingUI;

	TWeakObjectPtr<UAPAudioSetting> AudioSettingUI;


};
