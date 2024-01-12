// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArcanePunkCharacter.h"
#include "Blueprint/UserWidget.h"
#include "APEnhanceChoice.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPEnhanceChoice : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void InitType(ESkillTypeState UpdateSkillTypeState, EEnHanceType UpdateEnHanceType);

private:	
	void InitSuffle();
	void SetPauseGame();
	void TextSetting();
	void BindButton();

	UFUNCTION()
	void OnEnhanveChoice1();
	UFUNCTION()
	void OnEnhanveChoice2();
	UFUNCTION()
	void OnEnhanveChoice3();
	UFUNCTION()
	void OnReroll();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Choice1_Button;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Choice2_Button;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Choice3_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Choice1_Text;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Choice2_Text;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Choice3_Text;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SkillType_Text1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SkillType_Text2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SkillType_Text3;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* EnHanceType_Text;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Reroll_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* RerollDice_Text;

	UPROPERTY()
	AArcanePunkCharacter* OwnerCharacter;

	UPROPERTY()
	ESkillTypeState SkillTypeState = ESkillTypeState::Type_None;

	UPROPERTY()
	EEnHanceType EnHanceType = EEnHanceType::Silver;

	TArray<uint8> SkillNumber;


	uint8 Choice1;
	uint8 Choice2;
	uint8 Choice3;

	UPROPERTY()
	FName TEXT_EnhanceQ = TEXT("Q 강화");
	UPROPERTY()
	FName TEXT_EnhanceE = TEXT("E 강화");
	UPROPERTY()
	FName TEXT_EnhanceR = TEXT("R 강화");
	UPROPERTY()
	FName TEXT_Silver = TEXT("Silver");
	UPROPERTY()
	FName TEXT_Gold = TEXT("Gold");
	UPROPERTY()
	FName TEXT_Platinum = TEXT("Platinum");
};
