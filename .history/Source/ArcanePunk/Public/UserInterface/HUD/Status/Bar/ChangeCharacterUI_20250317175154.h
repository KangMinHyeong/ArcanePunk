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
	virtual void NativeConstruct() override;
	void Init(uint8 KeyNum, UTexture2D* Portrait);

	void ChangePlayer(bool IsPlayer);

	void StartSwapCoolDown(float CoolTime);
	bool CheckSwapCoolDown();

private:
	void EndSwapCoolDown();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* CurrentPlayerRing;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* PlayerImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SwapGauge;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_PlayerNum;

	bool CoolDown = true;

	FTimerHandle ChangeTimerHandle;
};
