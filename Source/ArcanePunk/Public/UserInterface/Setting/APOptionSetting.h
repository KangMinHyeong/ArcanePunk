// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APOptionSetting.generated.h"

class UButton;
class UTextBlock;
class UAPGraphicsSetting;
class UAPAudioSetting;
class UWidgetSwitcher;

UCLASS()
class ARCANEPUNK_API UAPOptionSetting : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) override;

	void BindButton();

	UFUNCTION()
	void OnClickButton_Graphics();
	UFUNCTION()
	void OnClickButton_Audio();
	UFUNCTION()
	void OnClickButton_Key();
	UFUNCTION()
	void OnClickButton_Screen();

	UFUNCTION()
	void OnClickButton_Back();

	// Option Setting
	// void InitGraphicsSetting();
	// void InitAudioSetting();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button_Graphics;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Audio;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Key;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Screen;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Back;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UWidgetSwitcher* Switcher_Option;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UWidget* GraphicsSettingUI;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UWidget* AudioSettingUI;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UWidget* SmartKeySettingUI;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UWidget* ScreenSettingUI;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Graphic;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Audio;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_SmartKey;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Screen;

	UPROPERTY(EditAnywhere)
	FLinearColor OnColor;
	UPROPERTY(EditAnywhere)
	FLinearColor OffColor;

	UPROPERTY(EditAnywhere)
	UTexture2D* TabImage_ON;
	UPROPERTY(EditAnywhere)
	UTexture2D* TabImage_OFF;
};
