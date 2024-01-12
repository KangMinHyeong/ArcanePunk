// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HomingTargetUI.generated.h"

class UCanvasPanel;
class UBorder;

UCLASS()
class ARCANEPUNK_API UHomingTargetUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void InputSkillInfo(uint8 UpdateSkillNumber, uint8 UpdateSkillType);

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	
	void HomingSkill();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* HomingCanvasPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* HomingBorder;

	uint8 SkillNumber = 0;
	uint8 SkillType = 0;

	UPROPERTY()
	class AArcanePunkCharacter* OwnerCharacter;
	UPROPERTY()
	class AArcanePunkPlayerController* OwnerCharacterPC;
};
