// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APTutorialUI.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UAPTutorialUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UWidget* HightLightUI;
};
