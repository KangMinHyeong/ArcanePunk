// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APSaveCompleteUI.generated.h"

class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPSaveCompleteUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_SaveComplete;

	FName SaveComplete = TEXT("SaveComplete");
};
