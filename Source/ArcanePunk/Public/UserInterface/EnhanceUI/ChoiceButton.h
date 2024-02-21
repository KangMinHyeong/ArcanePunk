// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "Blueprint/UserWidget.h"
#include "ChoiceButton.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class ARCANEPUNK_API UChoiceButton : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetEnhance(UUserWidget* UpdateParentWidget, ESkillNumber UpdateSkillNumber ,ESkillAbility UpdateSkillAbility);
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

	UPROPERTY()
	ESkillNumber SkillNumber = ESkillNumber::None;

	UPROPERTY()
	ESkillAbility SkillAbility = ESkillAbility::Ability_None;

	UPROPERTY()
	UUserWidget* ParentWidget;

	UPROPERTY()
	bool bEnhance;

};
