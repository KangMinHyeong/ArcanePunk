// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APSwapBarUI.generated.h"

class UChangeCharacterUI;
class UWrapBox;

UCLASS()
class ARCANEPUNK_API UAPSwapBarUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(TArray<uint8> UI_ID);
	void Active(int32 PlayerID);
	void Deactive(int32 PlayerID);

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayCannotSwitchingMessage();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWrapBox* WrapBox;

	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UChangeCharacterUI> SwapUIClass;

	UPROPERTY()
	TArray<UChangeCharacterUI*> SwapUIs;
	// UPROPERTY(EditAnywhere, meta = (BindWidget))
	// UChangeCharacterUI* SwapUI_1;

	// UPROPERTY(EditAnywhere, meta = (BindWidget))
	// UChangeCharacterUI* SwapUI_2;

	// UPROPERTY(EditAnywhere, meta = (BindWidget))
	// UChangeCharacterUI* SwapUI_3;
};
