// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChangeCharacterUI.generated.h"

class UBorder;

UCLASS()
class ARCANEPUNK_API UChangeCharacterUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void ChangePlayer() override;
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* CurrentPlayerRing;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SwapGauge;
};
