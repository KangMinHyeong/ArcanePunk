// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APSaveSlotUI.generated.h"

class UButton;
class UScrollBox;
class UAPSaveDataSlot;

UCLASS()
class ARCANEPUNK_API UAPSaveSlotUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseWheel( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	void BindButton();
	void BindSlot();

private:
	void OnSlot1();
	void OnSlot2();
	void OnSlot3();
	void OnSlot4();
	void OnSlot5();

	UFUNCTION()
	void OnClickButton_Back();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Back;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UScrollBox* ScrollBox;

	UPROPERTY(EditAnywhere)	
	TSubclassOf<UAPSaveDataSlot> SaveDataSlotClass;

	UPROPERTY(EditAnywhere)	
	TSubclassOf<UAPSaveDataSlot> NoDataSlotClass;

	UPROPERTY()
	bool IsTitle = false;
};
