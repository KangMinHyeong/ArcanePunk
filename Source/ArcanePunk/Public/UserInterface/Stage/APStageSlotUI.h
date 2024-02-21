// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APStageSlotUI.generated.h"

class UTextBlock;
class UWidgetSwitcher;

UCLASS()
class ARCANEPUNK_API UAPStageSlotUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDoubleClick( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

private:
	void CheckLock();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* Switcher_CanEnter;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidget* Unlocked;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidget* Locked;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TEXT_StageNumber;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TEXT_SubStageNumber;

	UPROPERTY(EditAnywhere)
	uint8 StageNumber = 0;

	UPROPERTY(EditAnywhere)
	uint8 SubStageNumber = 0;

	UPROPERTY()
	bool IsOpen = false;

	UPROPERTY(EditAnywhere)
	FName BattleStageLevelName = "";
};
