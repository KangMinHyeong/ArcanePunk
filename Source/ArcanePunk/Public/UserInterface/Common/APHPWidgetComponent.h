// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APHPWidgetComponent.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class ARCANEPUNK_API UAPHPWidgetComponent : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;	

	UFUNCTION()
	void UpdatingMaxHPAndHP(float UpdateMaxHP, float UpdateHP);
	void InitMaxHPAndHP(float UpdateMaxHP, float UpdateHP); 
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* HPPercent;

	// UPROPERTY(EditAnywhere, meta = (BindWidget))
	// UTextBlock* MaxHP_Text;

	// UPROPERTY(EditAnywhere, meta = (BindWidget))
	// UTextBlock* HP_Text;

	float MaxHP = 100.0f;
	float TargetHP = 100.0f;
	float CurrentHP = 100.0f;

	UPROPERTY(EditAnywhere)
	float HPBarSpeed = 25.0f;
};
