// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Blueprint/UserWidget.h"
#include "APSkillWindowSlot.generated.h"

class UTextBlock;
class UButton;
class UImage;
class AArcanePunkCharacter;
class UAPGameInstance;
class UStackBox;

UCLASS()
class ARCANEPUNK_API UAPSkillWindowSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void InitSkillCategory(UUserWidget* Parent, EEnhanceCategory UpdateEnhanceCategory);
	void InitPassiveCategory(UUserWidget* Parent, uint8 UpdatePassiveNumber);

private:
	void InitSkillImage();
	void InitEnhance();

	UFUNCTION()
	void OnSkillInformation();
	
protected:	
	TWeakObjectPtr<UUserWidget> ParentWidget;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_SkillName;	

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UButton* Button_Skill;	

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UStackBox* StackBox_Skill;

	FSkillAbilityNestingData EnhanceData;
	EEnhanceCategory EnhanceCategory;
	uint8 SkillNumber = 0;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
	TWeakObjectPtr<UAPGameInstance> APGI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> EnhanceSlotClass;

	FString SkillRowName;

	FSkillNameList* SkillNameTable;

	float CoolTime = -1.0f;
};
