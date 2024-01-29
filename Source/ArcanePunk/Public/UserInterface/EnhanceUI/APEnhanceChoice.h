// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArcanePunkCharacter.h"
#include "Blueprint/UserWidget.h"
#include "APEnhanceChoice.generated.h"

class UButton;
class UTextBlock;
class UWrapBox;
class UChoiceButton;

UCLASS()
class ARCANEPUNK_API UAPEnhanceChoice : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void InitType(ESkillKey UpdateEnhanceSkill, EEnHanceType UpdateEnHanceType);

	void ApplyEnhance(ESkillAbility UpdateSkillAbility);
	void ApplyNewSkill(ESkillNumber UpdateSkillNumber);

private:	
	void EnhanceSuffle();
	bool SkillSuffle();
	void SetPauseGame();
	void TextSetting();
	void SetAbility();
	void EnhanceListing();
	void SetChoiceButton();

	UFUNCTION()
	void OnReroll();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* EnHanceType_Text;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Reroll_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* RerollDice_Text;

	UPROPERTY()
	AArcanePunkCharacter* OwnerCharacter;

	UPROPERTY()
	ESkillKey EnhanceSkill = ESkillKey::None;

	UPROPERTY()
	ESkillNumber SkillNumber = ESkillNumber::None;

	UPROPERTY()
	ESkillNumber NewSkillNumber = ESkillNumber::None;

	UPROPERTY()
	EEnHanceType EnHanceType = EEnHanceType::Silver;

	UPROPERTY()
	TArray<uint8> SkillAbilities;

	UPROPERTY()
	TArray<uint8> NewSkills;

	ESkillAbility SkillAbility = ESkillAbility::Ability_None;

	UPROPERTY()
	TArray<ESkillAbility> EnableSkillAbilityList;

	UPROPERTY()
	TArray<ESkillAbility> OriginSkillAbilityList;
	
	uint8 Choice1;
	uint8 Choice2;
	uint8 Choice3;

	UPROPERTY()
	FName TEXT_Silver = TEXT("Silver");
	UPROPERTY()
	FName TEXT_Gold = TEXT("Gold");
	UPROPERTY()
	FName TEXT_Platinum = TEXT("Platinum");

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UWrapBox* ChoiceWrapBox;

	UPROPERTY()
	TArray<UChoiceButton*> ChoiceSlots;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UChoiceButton> ChoiceSlotClass;

	UPROPERTY(EditAnywhere)
	float SkillAppearPercent = 0.0f; // 스킬 등장 확률, 100 - 스킬 등장 확률 = 증강 등장 확률
};
