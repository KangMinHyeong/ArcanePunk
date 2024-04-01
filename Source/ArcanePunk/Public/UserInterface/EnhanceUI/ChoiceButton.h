// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "Blueprint/UserWidget.h"
#include "ChoiceButton.generated.h"

class UButton;
class UTextBlock;
class UAPEnhanceChoice;

UCLASS()
class ARCANEPUNK_API UChoiceButton : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetEnhance(UUserWidget* UpdateParentWidget, ESkillNumber UpdateSkillNumber, uint8 UpdateSkillAbility ,uint16 UpdateNestingNumb);
	void SetNewSkill(UUserWidget* UpdateParentWidget, ESkillNumber UpdateSkillNumber);

private:
	void BindButton();

	UFUNCTION()
	void OnEnhanceChoice();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Choice_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SkillNumber_Text;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SkillAbility_Text;

	ESkillNumber SkillNumber = ESkillNumber::None;

	// ESkillAbility SkillAbility = ESkillAbility::Ability_None;

	uint8 SkillAbility;
	uint16 NestingNumb;

	TWeakObjectPtr<UAPEnhanceChoice> ParentWidget;

	bool bEnhance;
};
