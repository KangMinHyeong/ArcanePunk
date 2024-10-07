// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/Common/Check/CheckingWidgetInterface.h"
#include "Blueprint/UserWidget.h"
#include "APSaveSlotUI.generated.h"

class UButton;
class UScrollBox;
class UAPSaveDataSlot;
class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPSaveSlotUI : public UUserWidget, public ICheckingWidgetInterface
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)override;
	virtual FReply NativeOnMouseButtonDoubleClick( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnMouseWheel( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) override;
	
	virtual void OnValidating(ECheckType UpdateCheckType) override;
	
	void BindButton();
	void BindSlot();

	void ChangingCurrentSaveSlot(UUserWidget* ClickedSlot);

private:
	void OnSlot();

	UFUNCTION()
	void OnClickButton_Back();
	UFUNCTION()
	void OnClickButton_Delete();
	UFUNCTION()
	void OnClickButton_Select();

	void OpenCheckUI(ECheckType UpdateCheckType);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Back;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Delete;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Select;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UScrollBox* ScrollBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_Select;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_Delete;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* TextBlock_SaveSlot;

	UPROPERTY(EditAnywhere)	
	TSubclassOf<UAPSaveDataSlot> SaveDataSlotClass;

	UPROPERTY(EditAnywhere)	
	TSubclassOf<UAPSaveDataSlot> NoDataSlotClass;

	TWeakObjectPtr<UAPSaveDataSlot> CurrentSaveSlot;

	UPROPERTY(EditAnywhere)	
	TArray<FString> SlotNames;

	bool IsTitle = false;

};
