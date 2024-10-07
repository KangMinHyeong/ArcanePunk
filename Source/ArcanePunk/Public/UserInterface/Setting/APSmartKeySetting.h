// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APSmartKeySetting.generated.h"

class UCanvasPanel;
class UBorder;
class UButton;
class UWidgetSwitcher;
class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPSmartKeySetting : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
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
	void OnSmartKeyQ_Off();
	UFUNCTION()
	void OnSmartKeyE_Off();
	UFUNCTION()
	void OnSmartKeyR_Off();

	UFUNCTION()
	void OnApply();
	UFUNCTION()
	void OnCancel();

	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SmartKeyQ_Button_On;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SmartKeyE_Button_On;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SmartKeyR_Button_On;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SmartKeyQ_Button_Off;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SmartKeyE_Button_Off;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SmartKeyR_Button_Off;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SmartKeyQ_Text_On;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SmartKeyE_Text_On;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SmartKeyR_Text_On;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SmartKeyQ_Text_Off;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SmartKeyE_Text_Off;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SmartKeyR_Text_Off;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextBlock_Q;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextBlock_E;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextBlock_R;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextBlock_Init; 
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextBlock_Apply; 

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Apply_Button;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RollBack_Button;

	UPROPERTY(EditAnywhere)
	FLinearColor TextOnColor;
	UPROPERTY(EditAnywhere)
	FLinearColor TextOffColor;

	UPROPERTY(EditAnywhere)
	UTexture2D* TabImage_ON;
	UPROPERTY(EditAnywhere)
	UTexture2D* TabImage_OFF;
	
	TArray<bool> CopySmartKeyArr;
};
