// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APStageSelectingUI.generated.h"

class UWidgetSwitcher;
class UButton;

UCLASS()
class ARCANEPUNK_API UAPStageSelectingUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void BindButton();

	UFUNCTION()
	void OnClickButton_Chapter1();
	UFUNCTION()
	void OnClickButton_Chapter2();
	UFUNCTION()
	void OnClickButton_Chapter3();

	UFUNCTION()
	void OnClickButton_Back();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* Switcher_StageSlot;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button_Chapter1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button_Chapter2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button_Chapter3;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button_Back;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidget* StageSlot_1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidget* StageSlot_2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidget* StageSlot_3;
};
