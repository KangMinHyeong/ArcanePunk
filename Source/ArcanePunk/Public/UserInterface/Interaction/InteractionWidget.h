// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class AArcanePunkCharacter;
struct FInteractData;
class UTextBlock;
class UProgressBar;

UCLASS()
class ARCANEPUNK_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	float UpdateInteractionProgress();

	void UpdateWidget(FInteractData InteractData);

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category="Interaction Widget | Interactable Data")
	UTextBlock* NameText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* KeyPressText;

	// UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	// UProgressBar* InteractionProgressBar;

	float CurrentInteractionDuration = 0.0f;;
	
};
