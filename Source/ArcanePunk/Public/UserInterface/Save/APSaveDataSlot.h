// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APSaveDataSlot.generated.h"

class UTextBlock;
class UAPSaveSlotUI;
class UBorder;
class UAPSaveGame;

UCLASS()
class ARCANEPUNK_API UAPSaveDataSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)override;
	virtual FReply NativeOnMouseButtonDoubleClick( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

	void SetSlotData(UAPSaveGame* SavedData);
	void SetSlotName(FString PlayerSlot, UUserWidget* Parent);
	void SetSlotNumber(uint8 NewSlotNumber);

	void TurnOnSlot();
	void TurnOffSlot();

	void Save();
	void Load();
	void Delete();
private:

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UBorder* Border_Background;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TEXT_SlotName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TEXT_SlotNumber;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TEXT_PlayTime_Hours;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TEXT_PlayTime_Minutes;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TEXT_PlayTime_Seconds;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TEXT_Year;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TEXT_Month;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TEXT_Day;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TEXT_Hours;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TEXT_Minutes;

	FString SlotName = "No Data";

	FString PlayerSlotName = TEXT("PlayerSlot_1");
	FString DefaultPlayerSlot = TEXT("PlayerSlot_0");

	TWeakObjectPtr<UAPSaveSlotUI> ParentWidget;

	UPROPERTY(EditAnywhere)
	UTexture2D* TabImage_ON;
	UPROPERTY(EditAnywhere)
	UTexture2D* TabImage_OFF;

	bool HasSavingData = false;
};
