// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InteractionInterface.h"
#include "Blueprint/UserWidget.h"
#include "APConversationUI.generated.h"

class UAPConversationText;
class UAPTutorialUI;

UCLASS()
class ARCANEPUNK_API UAPConversationUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	
	virtual FReply NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) override;
	virtual FReply NativeOnMouseButtonDoubleClick( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	void InitOrder(const int32 & NewGroupID, bool bTutorial = false, UAPTutorialUI* UI = nullptr);
	
	void StartTextFlow();
private:
	void ConversationCompleteCheck();
	void SetOrder();

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UAPConversationText* ConversationTextUI;

	int32 GroupID;

	bool bTutorialUI = false;
	TWeakObjectPtr<UAPTutorialUI> TutorialUI;

	UPROPERTY()
	TArray<FDialogueDataTable> Dialogues;
	UPROPERTY()
	UDataTable* CharacterUIData;
	
};
