// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Blueprint/UserWidget.h"
#include "ImitatorSkillSlot.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class ARCANEPUNK_API UImitatorSkillSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void UpdateSkillSlot(uint8 SkillNumber);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSkillAction_FadeIn();
	UFUNCTION(BlueprintImplementableEvent)
	void OnSkillAction_FadeOut();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SkillImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Text_SkillName;

	UPROPERTY(EditAnywhere)
	UDataTable* SkillNameData;

	UPROPERTY(EditAnywhere)
	FName Name = TEXT("NormalSkill");
};
