// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChangeCharacterUI.generated.h"

class UBorder;
class UImage;
class UTextBlock;

UCLASS()
class ARCANEPUNK_API UChangeCharacterUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init() override;

	void ChangePlayer(bool IsPlayer);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* CurrentPlayerRing;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SwapGauge;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_PlayerNum;

	UPROPERTY(EditAnywhere)
	uint8 PlayerNum = 1;
};
