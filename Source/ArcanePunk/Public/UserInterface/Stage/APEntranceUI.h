// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APEntranceUI.generated.h"

class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPEntranceUI : public UUserWidget
{
	GENERATED_BODY()
public:	
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_LevelName;
};
