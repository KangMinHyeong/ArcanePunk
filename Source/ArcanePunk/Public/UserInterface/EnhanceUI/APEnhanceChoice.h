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

	void InitType(EEnhanceCategory UpdateEnhanceCategory, EEnHanceType UpdateEnHanceType);

	void ApplyEnhance(uint8 UpdateSkillAbility, uint16 UpdateNestingNumb );
	void ApplyNewSkill(ESkillNumber UpdateSkillNumber);
	FORCEINLINE UDataTable* GetSkillAbilityDataTable() const {return SkillAbilityDataTable;};
	FORCEINLINE EEnHanceType GetEnHanceType() const {return EnHanceType;};
	FORCEINLINE FSkillAbilityNestingData GetSkillAbilityNestingData() const {return SkillAbilityNestingData;};
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
	UTextBlock* EnHanceCategory_Text;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Reroll_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* RerollDice_Text;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;

	EEnhanceCategory EnhanceCategory = EEnhanceCategory::None;

	ESkillNumber SkillNumber = ESkillNumber::None;

	ESkillNumber NewSkillNumber = ESkillNumber::None;

	EEnHanceType EnHanceType = EEnHanceType::Silver;

	TArray<uint8> SkillAbilities;

	TArray<uint8> NewSkills;

	// ESkillAbility SkillAbility = ESkillAbility::Ability_None;

	// TArray<ESkillAbility> EnableSkillAbilityList;

	// TArray<ESkillAbility> OriginSkillAbilityList;
	
	uint8 Choice1;
	uint8 Choice2;
	uint8 Choice3;

	FName TEXT_Silver = TEXT("Silver");
	FName TEXT_Gold = TEXT("Gold");
	FName TEXT_Platinum = TEXT("Platinum");

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UWrapBox* ChoiceWrapBox;

	UPROPERTY()
	TArray<UChoiceButton*> ChoiceSlots;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UChoiceButton> ChoiceSlotClass;

	UPROPERTY(EditAnywhere)
	float SkillAppearPercent = 0.0f; // 스킬 등장 확률, 100 - 스킬 등장 확률 = 증강 등장 확률

	UPROPERTY(EditAnywhere)
	UDataTable* SkillAbilityDataTable;

	FSkillAbilityNestingData SkillAbilityNestingData;

	uint16 PlusAbilityNum = 1;
};
