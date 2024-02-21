// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APSaveDataSlot.generated.h"

class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPSaveDataSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDoubleClick( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

	void SetSlotName(FString NewName);
	void SetSlotNumber(uint8 NewSlotNumber);
	void SetLoadMode(bool NewBool) {LoadMode = NewBool;};

private:
	void Save();
	void Load();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TEXT_SlotName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TEXT_SlotNumber;

	UPROPERTY()
	FString SlotName = "No Data";

	UPROPERTY()
	uint8 SlotNumber = 0;

	UPROPERTY()
	FString PlayerSlotName_1 = TEXT("PlayerSlot_1");

	UPROPERTY()
	FString PlayerSlotName_2 = TEXT("PlayerSlot_2");

	UPROPERTY()
	FString PlayerSlotName_3 = TEXT("PlayerSlot_3");

	UPROPERTY()
	FString PlayerSlotName_4 = TEXT("PlayerSlot_4");

	UPROPERTY()
	FString PlayerSlotName_5 = TEXT("PlayerSlot_5");

	UPROPERTY()
	FString DefaultPlayerSlot = TEXT("PlayerSlot_0");

	UPROPERTY()
	FString GameSlotName_1 = TEXT("GameSlot_1");

	UPROPERTY()
	FString GameSlotName_2 = TEXT("GameSlot_2");

	UPROPERTY()
	FString GameSlotName_3 = TEXT("GameSlot_3");

	UPROPERTY()
	FString GameSlotName_4 = TEXT("GameSlot_4");

	UPROPERTY()
	FString GameSlotName_5 = TEXT("GameSlot_5");

	UPROPERTY()
	FString DefaultGameSlot = TEXT("GameSlot_0");

	UPROPERTY()
	bool LoadMode = false; // T : Title and Load , F : InGame and Save
};
