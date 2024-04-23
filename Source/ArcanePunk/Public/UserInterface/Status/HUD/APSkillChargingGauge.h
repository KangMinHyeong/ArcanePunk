// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APSkillChargingGauge.generated.h"

class UProgressBar;

UCLASS()
class ARCANEPUNK_API UAPSkillChargingGauge : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	void SetSkillChargingPercent(float Current, float Max);	
	void ChargeEnd(bool Start);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* ChargingGauge;

};
