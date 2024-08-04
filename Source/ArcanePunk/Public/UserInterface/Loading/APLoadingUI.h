// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APLoadingUI.generated.h"

class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPLoadingUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;	

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_Loading;

	FName Loading = TEXT("Loading");
};
