// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APSmartKeySetting.generated.h"

class UCanvasPanel;
class UBorder;
class UButton;
class UWidgetSwitcher;

UCLASS()
class ARCANEPUNK_API UAPSmartKeySetting : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	void InitSmartKey();

	void BindSmartKeyButton();

	UFUNCTION()
	void OnSmartKeyQ();
	UFUNCTION()
	void OnSmartKeyE();
	UFUNCTION()
	void OnSmartKeyR();
	UFUNCTION()
	void OnApply();
	UFUNCTION()
	void OnCancel();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* SmartKeyCanvasPanel;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* SmartKeyBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SmartKeyQ_Button;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SmartKeyE_Button;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SmartKeyR_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Apply_Button;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Cancel_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* SmartKeyQ_Switcher;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* SmartKeyE_Switcher;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* SmartKeyR_Switcher;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidget* EnableSmartKeyQ;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidget* DisableSmartKeyQ;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidget* EnableSmartKeyE;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidget* DisableSmartKeyE;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidget* EnableSmartKeyR;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidget* DisableSmartKeyR;
	
	TArray<bool> CopySmartKeyArr;
};
