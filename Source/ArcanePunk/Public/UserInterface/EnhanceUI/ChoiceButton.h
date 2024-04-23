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
class UImage;

UCLASS()
class ARCANEPUNK_API UChoiceButton : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetEnhance(UUserWidget* UpdateParentWidget, ESkillNumber UpdateSkillNumber, uint8 UpdateSkillAbility ,uint16 UpdateNestingNumb);
	void SetNewSkill(UUserWidget* UpdateParentWidget, ESkillNumber UpdateSkillNumber);
	FORCEINLINE void SetChoiceIndexNum(uint8 NewValue) {ChoiceIndexNum = NewValue;};

private:
	void BindButton();

	UFUNCTION()
	void OnEnhanceChoice();

	UFUNCTION()
	void OnReroll();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Choice_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Reroll_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SkillNumber_Text;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SkillAbility_Text;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SkillAbility_Nesting;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SkillNumber_Image;

	UPROPERTY(EditAnywhere)
	UDataTable* SkillSlotDataTable;

	UPROPERTY(EditAnywhere)
	FName Name = TEXT("NormalSkill");
	// ESkillAbility SkillAbility = ESkillAbility::Ability_None;

	uint8 SkillAbility;
	uint16 NestingNumb;

	ESkillNumber SkillNumber = ESkillNumber::None;

	TWeakObjectPtr<UAPEnhanceChoice> ParentWidget;

	bool bEnhance;

	uint8 ChoiceIndexNum = 0; // 0, 1, 2 IndexNumber

	uint16 MaxNestingNum = 1;
};
