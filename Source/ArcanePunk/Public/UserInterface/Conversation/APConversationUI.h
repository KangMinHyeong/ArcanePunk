// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InteractionInterface.h"
#include "Blueprint/UserWidget.h"
#include "APConversationUI.generated.h"

class UAPConversationText;

UCLASS()
class ARCANEPUNK_API UAPConversationUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	
	virtual FReply NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) override;

	void InitOrder(FName Name);
	
private:
	void ConversationCompleteCheck();
	void SetOrder();

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UAPConversationText* ConversationText;

	FName RowName;
};
