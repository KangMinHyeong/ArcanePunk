// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APShopListTitle.generated.h"

class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPShopListTitle : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_GoodsName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_GoodsNum;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_GoodsPrice;
	
};
