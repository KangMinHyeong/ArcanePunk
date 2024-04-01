// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Blueprint/UserWidget.h"
#include "APSkillSlot.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPSkillSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual void UpdateSkillSlot(uint8 SkillNumber);
	UFUNCTION(BlueprintImplementableEvent)
	void OnHightLight();
	UFUNCTION(BlueprintImplementableEvent)
	void OnOperating();
	void SetUsingSkill(bool NewBool);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SkillImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_SkillKey;

	UPROPERTY(EditAnywhere)
	UDataTable* SkillSlotDataTable;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SkillUsingImage;

	UPROPERTY(EditAnywhere)
	FName Name = TEXT("NormalSkill");

	UPROPERTY(EditAnywhere)
	FString SkillKey = TEXT("Q");
};
