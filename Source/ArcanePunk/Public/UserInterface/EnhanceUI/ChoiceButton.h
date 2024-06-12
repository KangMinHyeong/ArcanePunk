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
class USizeBox;

UCLASS()
class ARCANEPUNK_API UChoiceButton : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetEnhance(UUserWidget* UpdateParentWidget, uint8 UpdateSkillNumber, uint8 UpdateSkillAbility ,uint16 UpdateNestingNumb);
	void SetNewSkill(UUserWidget* UpdateParentWidget, uint8 UpdateSkillNumber);
	FORCEINLINE void SetChoiceIndexNum(uint8 NewValue) {ChoiceIndexNum = NewValue;};

	UFUNCTION(BlueprintImplementableEvent)
	void OnChoice_FadeIn();
	UFUNCTION(BlueprintImplementableEvent)
	void OnRerollButton_FadeIn();

	UFUNCTION(BlueprintImplementableEvent)
	void OnChoiceButton_Hovered();
	UFUNCTION(BlueprintImplementableEvent)
	void OnChoiceButton_UnHovered();

	UFUNCTION(BlueprintImplementableEvent)
	void OnRerollButton_Hovered();
	UFUNCTION(BlueprintImplementableEvent)
	void OnRerollButton_UnHovered();

	UFUNCTION(BlueprintImplementableEvent)
	void OnEnhanceChoice();

private:
	void BindButton();
	void SetSkillName();

	UFUNCTION()
	void OnClickChoice();
	UFUNCTION(BlueprintCallable)
	void OnReroll();
	UFUNCTION(BlueprintCallable)
	void ApplyChoice();

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
	UTextBlock* Text_New;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* SizeBox_Nesting;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SkillNumber_Image;

	UPROPERTY(EditAnywhere)
	UDataTable* SkillNameDataTable;

	FName Name = TEXT("NormalSkill");
	FName SkillNumberName;
	// ESkillAbility SkillAbility = ESkillAbility::Ability_None;

	uint8 SkillAbility;
	uint16 NestingNumb;
	FSkillAbilityNestingData NestingData;

	uint8 SkillNumber = 0;

	TWeakObjectPtr<UAPEnhanceChoice> ParentWidget;

	bool bEnhance;

	uint8 ChoiceIndexNum = 0; // 0, 1, 2 IndexNumber

	uint16 MaxNestingNum = 1;
};
