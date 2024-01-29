// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "HomingTargetUI.generated.h"

class UCanvasPanel;
class UBorder;

UCLASS()
class ARCANEPUNK_API UHomingTargetUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void InputSkillInfo(ESkillNumber UpdateSkillNumber);

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	
	void HomingSkill();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* HomingCanvasPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* HomingBorder;

	ESkillNumber SkillNumber = ESkillNumber::None;

	UPROPERTY()
	AArcanePunkCharacter* OwnerCharacter;
	UPROPERTY()
	AArcanePunkPlayerController* OwnerCharacterPC;
};
