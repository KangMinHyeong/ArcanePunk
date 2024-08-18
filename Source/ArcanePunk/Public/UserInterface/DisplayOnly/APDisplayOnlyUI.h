// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APDisplayOnlyUI.generated.h"

class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPDisplayOnlyUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnEndedUIAnim();

	virtual void OnEndedUIAnim_Implementation();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_1;

	UPROPERTY(EditAnywhere)
	FName Text_1;	
};
