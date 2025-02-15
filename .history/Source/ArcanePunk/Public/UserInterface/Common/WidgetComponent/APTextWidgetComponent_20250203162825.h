// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APTextWidgetComponent.generated.h"

class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPTextWidgetComponent : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	void SetDamageText(float AppliedDamage, bool bCritical = false);
	void SetDetectText();

	void SetDialogueText();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageAction();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDetectAction();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_Content;	

	UPROPERTY(EditAnywhere)
	FSlateColor Basic_Color;

	UPROPERTY(EditAnywhere)
	FSlateColor Critical_Color;

	UPROPERTY(EditAnywhere)
	FSlateColor Detect_Color;

	UPROPERTY(EditAnywhere)
	FString DetectText = "!!";
};
