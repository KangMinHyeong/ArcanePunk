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

	FString SlotName = "No Data";

	uint8 SlotNumber = 0;

	FString PlayerSlotName_1 = TEXT("PlayerSlot_1");

	FString PlayerSlotName_2 = TEXT("PlayerSlot_2");

	FString PlayerSlotName_3 = TEXT("PlayerSlot_3");

	FString PlayerSlotName_4 = TEXT("PlayerSlot_4");

	FString PlayerSlotName_5 = TEXT("PlayerSlot_5");

	FString DefaultPlayerSlot = TEXT("PlayerSlot_0");

	FString GameSlotName_1 = TEXT("GameSlot_1");

	FString GameSlotName_2 = TEXT("GameSlot_2");

	FString GameSlotName_3 = TEXT("GameSlot_3");

	FString GameSlotName_4 = TEXT("GameSlot_4");

	FString GameSlotName_5 = TEXT("GameSlot_5");

	FString DefaultGameSlot = TEXT("GameSlot_0");

	bool LoadMode = false; // T : Title and Load , F : InGame and Save
};
