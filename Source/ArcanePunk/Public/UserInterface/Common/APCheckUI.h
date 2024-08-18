// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/Common/CheckingWidgetInterface.h"
#include "Blueprint/UserWidget.h"
#include "APCheckUI.generated.h"

class UButton;
class UTextBlock;


UCLASS()
class ARCANEPUNK_API UAPCheckUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetCheckType(ECheckType UpdateCheckType, UUserWidget* Parent);

private:
	UFUNCTION()
	void OnValidate();

	UFUNCTION()
	void OnCancel();

private:
	TWeakObjectPtr<UUserWidget> ParentWidget;

	ECheckType CurrentCheckType;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button_Validation;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button_Cancel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_Validation;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_Cancel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_Message;


};
