// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InteractionInterface.h"
#include "Blueprint/UserWidget.h"
#include "APSystemMessage.generated.h"

class UTextBlock;
class USizeBox;

UCLASS()
class ARCANEPUNK_API UAPSystemMessage : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	void DisplaySystemMesseage(EStringRowName StringRowName, bool bLowMessage, bool bWarning);

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* Text_Low;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* Text_High;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	USizeBox* Content_Low;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	USizeBox* Content_High;

	UPROPERTY(EditAnywhere)
	FSlateColor Notice_Color;
	UPROPERTY(EditAnywhere)
	FSlateColor Warning_Color;

};
