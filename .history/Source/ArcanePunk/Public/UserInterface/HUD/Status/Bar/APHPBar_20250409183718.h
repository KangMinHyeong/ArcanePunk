// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APHPBar.generated.h"

class UTextBlock;
class UProgressBar;
class AArcanePunkCharacter;
class UAPShieldHPBar;

UCLASS()
class ARCANEPUNK_API UAPHPBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetMaxHP(float NewValue);
	void SetHPPercent(AArcanePunkCharacter* Character, float Origin);
	void UpdatePercentBar(float InDeltaTime);

	float AddSwapGauge(float Add);
	void UpdateSwapGauge(float InDeltaTime);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* HPBar;	

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TEXT_CurrentHP;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TEXT_CurrentMaxHP;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAPShieldHPBar* ShieldHPBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SwapGauge;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;

	float OriginHP = 0.0f;

	bool bChange = false;

	float BarSpeed = 17.5f;

	UPROPERTY(EditAnywhere)
	float SwapGaugeSpeed = 2.0f;
	float CurrentSwapGaugeSpeed = 1.0f;

	float CurrentSwapGauge = 0.0f;
	float TargetSwapGauge = 0.0f;
};
