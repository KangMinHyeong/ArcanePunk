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

	FORCEINLINE float GetMasterVolume() const { return MasterVolume;};
	FORCEINLINE float GetBGMVolume() const { return BGMVolume;};
	FORCEINLINE float GetEffectVolume() const { return EffectVolume;};

	FORCEINLINE void SetMasterVolume(float NewValue) { MasterVolume = NewValue;};
	FORCEINLINE void SetBGMVolume(float NewValue) { BGMVolume = NewValue;};
	FORCEINLINE void SetEffectVolume(float NewValue) { EffectVolume = NewValue;};

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

	UPROPERTY()
	UAPGraphicsSetting* GraphicsSettingUI;

	UPROPERTY()
	UAPAudioSetting* AudioSettingUI;
	UPROPERTY(EditAnywhere)
	float MasterVolume = 1.0f;
	UPROPERTY(EditAnywhere)
	float BGMVolume = 1.0f;
	UPROPERTY(EditAnywhere)
	float EffectVolume = 1.0f;

};
