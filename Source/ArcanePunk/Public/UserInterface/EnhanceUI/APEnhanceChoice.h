// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArcanePunkCharacter.h"
#include "Blueprint/UserWidget.h"
#include "APEnhanceChoice.generated.h"

class UButton;
class UTextBlock;
class UHorizontalBox;
class UChoiceButton;
class USizeBox;
class UAPGameInstance;
class UBorder;

UCLASS()
class ARCANEPUNK_API UAPEnhanceChoice : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void InitType(EEnhanceCategory UpdateEnhanceCategory, EEnHanceType UpdateEnHanceType, uint8 UpdateEnhanceSkillNum);

	void ApplyEnhance(uint8 UpdateSkillNumber, uint8 UpdateSkillAbility, uint16 UpdateNestingNumb, uint16 MaxNestingNumb);
	void ApplyNewSkill(uint8 UpdateSkillNumber);
	FORCEINLINE UDataTable* GetSkillAbilityRowNameData() const {return SkillAbilityRowNameData;};
	FORCEINLINE EEnhanceCategory GetEnHanceCategory() const {return EnhanceCategory;};
	FORCEINLINE EEnHanceType GetEnHanceType() const {return EnHanceType;};
	FORCEINLINE FSkillAbilityNestingData GetSkillAbilityNestingData() const {return SkillAbilityNestingData;};
	FORCEINLINE FSkillAbilityNestingData GetPassiveAbilityNestingData(uint8 Key) const {return PassiveNestingData[Key];};
	FORCEINLINE void SetSkillAppearPercent(float NewValue) {SkillAppearPercent = NewValue; PassiveSkillAppearPercent = NewValue;};
	
	void OnReroll(uint8 ChoiceIndexNum);

	UFUNCTION(BlueprintImplementableEvent)
	void OnBackGround_FadeIn();
	UFUNCTION(BlueprintImplementableEvent)
	void OnBackGround_FadeOut();
	UFUNCTION(BlueprintImplementableEvent)
	void OnReroll_FadeIn();

	UFUNCTION(BlueprintCallable)
	void OnDisplayChoiceButton();
	UFUNCTION(BlueprintCallable)
	void OnDisplayRerollButton();

private:	
	void InitTypeSetting();
	void ActiveSuffle(uint8 Index);
	void InitPassiveSuffle(uint8 Index);
	void InitPassiveSkillName();
	
	void EnhanceSuffle(FName SkillName);
	bool SkillSuffle();
	bool PassiveSuffle();
	
	void SetPauseGame();
	void SetAbility();
	void EnhanceListing();
	void SetChoiceButton();

	UFUNCTION()
	void OnCancel();

	void OnPlayingChoiceButton(uint8 CurrentNum);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* ChoiceBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* EnHanceType_Text;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* EnHanceCategory_Text;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_CurrentReroll;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_Count;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Cancel_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* RerollDice_Text;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
	TWeakObjectPtr<UAPGameInstance> APGI;

	EEnhanceCategory EnhanceCategory = EEnhanceCategory::None;

	uint8 SkillNumber = 0;

	uint8 NewSkillNumber = 0;
	uint8 NewPassiveNumber = 0;

	EEnHanceType EnHanceType = EEnHanceType::Silver;

	TArray<uint8> SkillAbilities;

	TArray<uint8> NewSkills;
	TArray<uint8> NewPassives;
	
	uint8 Choice1;
	uint8 Choice2;
	uint8 Choice3;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	USizeBox* ChoiceBox_1;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	USizeBox* ChoiceBox_2;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	USizeBox* ChoiceBox_3;
	
	UPROPERTY()
	TArray<UChoiceButton*> ChoiceSlots;

	UPROPERTY()
	TArray<USizeBox*> ChoiceSizeBoxes;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UChoiceButton> ChoiceSlotClass;

	UPROPERTY(EditAnywhere)
	float SkillAppearPercent = 0.0f; // 스킬 등장 확률, 100 - 스킬 등장 확률 = 증강 등장 확률

	UPROPERTY(EditAnywhere)
	float PassiveSkillAppearPercent = 50.0f; // 스킬 등장 확률, 100 - 스킬 등장 확률 = 증강 등장 확률

	UPROPERTY(EditAnywhere)
	UDataTable* SkillAbilityRowNameData;

	FSkillAbilityNestingData SkillAbilityNestingData;

	TMap<uint8,FSkillAbilityNestingData> PassiveNestingData;

	TArray<FName> PassiveSkills;

	uint16 PlusAbilityNum = 1;
	uint8 EnhanceSkillNum = 0;

	UPROPERTY(EditAnywhere)
	FLinearColor SilverColor;
	UPROPERTY(EditAnywhere)
	FLinearColor GoldColor;
	UPROPERTY(EditAnywhere)
	FLinearColor PlatinumColor;

	uint8 ChoiceNumb = 0;

};
