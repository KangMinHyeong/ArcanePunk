// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APStatusBar.generated.h"

class UProgressBar;
class AAPHUD;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API UAPStatusBar : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void InitStatusBar();
	void SetHPPercent(float Origin);
	void UpdatePercentBar(float InDeltaTime);
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* HPBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* MPBar;

	float OriginHP = 0.0f;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;

	TWeakObjectPtr<AAPHUD>  HUD;

	bool bChange = false;

	UPROPERTY(EditAnywhere)
	float BarSpeed = 3.0f;
};
