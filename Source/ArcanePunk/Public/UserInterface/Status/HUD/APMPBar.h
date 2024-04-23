// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APMPBar.generated.h"

class UBorder;
class UImage;

UCLASS()
class ARCANEPUNK_API UAPMPBar : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void FadeOut();
	UFUNCTION(BlueprintImplementableEvent)
	void FadeIn();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* MP_Border;	

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* MPCharge;

};
