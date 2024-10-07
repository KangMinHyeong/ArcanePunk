// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APOtherSettings.generated.h"

class UButton;
class UTextBlock;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API UAPOtherSettings : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	void BindButton();
	void InitOtherSetting();

	UFUNCTION()
	void OnApply();
	UFUNCTION()
	void OnRollBack();

	UFUNCTION()
	void OnGuideBarOn();
	UFUNCTION()
	void OnGuideBarOff();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* GuideBar_On_Button;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* GuideBar_Off_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Apply_Button;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RollBack_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* GuideBarSetting_Text;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* GuideBar_On_Text;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* GuideBar_Off_Text;


	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextBlock_Init; 
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextBlock_Apply; 

	UPROPERTY(EditAnywhere)
	UTexture2D* TabImage_ON;
	UPROPERTY(EditAnywhere)
	UTexture2D* TabImage_OFF;

	bool bGuideBar = true;

	TWeakObjectPtr<AArcanePunkCharacter> Player;

	FButtonStyle On_ButtonStyle;
	FButtonStyle Off_ButtonStyle;

	UPROPERTY(EditAnywhere)
	FLinearColor TextOnColor;
	UPROPERTY(EditAnywhere)
	FLinearColor TextOffColor;
	
};
