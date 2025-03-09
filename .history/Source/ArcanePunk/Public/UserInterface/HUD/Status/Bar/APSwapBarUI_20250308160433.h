// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APSwapBarUI.generated.h"

class UChangeCharacterUI;

UCLASS()
class ARCANEPUNK_API UAPSwapBarUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void Active();
	void Deactive();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UChangeCharacterUI* SwapUI_1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UChangeCharacterUI* SwapUI_1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UChangeCharacterUI* SwapUI_1;
};
