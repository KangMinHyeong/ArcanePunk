// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SkillDataTable/SkillDataTable.h"
#include "Blueprint/UserWidget.h"
#include "APSkillWindowInformation.generated.h"

class UImage;
class UTextBlock;
class UStackBox;
class USizeBox;
class UAPGameInstance;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API UAPSkillWindowInformation : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void InitInformationData_Skill(EEnhanceCategory UpdateEnhanceCategory, FSkillNameList* SkillNameTable, float CoolTime);
	void InitInformationData_Enhance(EEnHanceType UpdateEnhanceType, FSkillAbilityDataSheet* UpdateAbilityData, uint16 UpdateNestingNumber);
private:
	void DisplayCoolTime(float CoolTime);
	
protected:	
	TWeakObjectPtr<UUserWidget> ParentWidget;
	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
	TWeakObjectPtr<UAPGameInstance> APGI;	

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UImage* Image_Skill;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UImage* Image_TierColor;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_Name;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_Category;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_Information;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_Ability;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_Ability_2;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_AbilityNum;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_AbilityNum_2;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_AbilityNum_End;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_AbilityNum_End_2;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UStackBox* StackBox_Information;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	USizeBox* SizeBox_Ability_1;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	USizeBox* SizeBox_Ability_2;

	UPROPERTY(EditAnywhere)
	FLinearColor SilverColor;
	UPROPERTY(EditAnywhere)
	FLinearColor GoldColor;
	UPROPERTY(EditAnywhere)
	FLinearColor PlatinumColor;

	FString CurrentAbility = TEXT("쿨타임 : ");
	FString CurrentNesting = TEXT("현재 중첩 : ");
	FString CurrentAbility_2 = TEXT("MP 소모량 : ");
	FString MaxNesting = TEXT("최대 중첩 : ");
	FString EndString_1 = TEXT("번");
	FString EndString_2 = TEXT("초");
	FString EndString_3 = TEXT("");
};
