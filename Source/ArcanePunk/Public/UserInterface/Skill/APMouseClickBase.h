// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "APMouseClickBase.generated.h"

class UCanvasPanel;
class UBorder;

// Prevent Other Mouse Click Event
UCLASS()
class ARCANEPUNK_API UAPMouseClickBase : public UUserWidget
{
	GENERATED_BODY()
public:
	void InputSkillInfo(ESkillNumber UpdateSkillNumber);

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	

	void SkillCancel();
	void SkillPlay();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PreventCanvasPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* PreventBorder;	

	ESkillNumber SkillNumber = ESkillNumber::None;

	UPROPERTY()
	AArcanePunkCharacter* OwnerCharacter;
	UPROPERTY()
	AArcanePunkPlayerController* OwnerCharacterPC;
};
