// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APMPBar.generated.h"

class UWidgetSwitcher;

UCLASS()
class ARCANEPUNK_API UAPMPBar : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void UseMP();
	void RecoveryMP();

	UFUNCTION(BlueprintImplementableEvent)
	void FadeOut();
	UFUNCTION(BlueprintImplementableEvent)
	void FadeIn();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* MPSwitcher;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidget* MP_Origin;	

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidget* MP_Use;	
};
