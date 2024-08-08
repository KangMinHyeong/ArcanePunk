// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APTitleUI.generated.h"

class UAPSaveSlotUI;
class UButton;
class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPTitleUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnNewGame();

	UFUNCTION()
	void OnContinue();

	UFUNCTION()
	void OnSetting();

	UFUNCTION()
	void OnExit();

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_NewGame;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Continue;	

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Setting;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Exit;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_NewGame;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_Continue;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_GameSetting;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_GameExit;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_MainTitle;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_SubTitle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPSaveSlotUI> SelectSaveSlotClass;
};
