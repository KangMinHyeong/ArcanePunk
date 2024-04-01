// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Blueprint/UserWidget.h"
#include "APStatusBar.generated.h"

class UProgressBar;
class AAPHUD;
class AArcanePunkCharacter;
class UHorizontalBox;
class UImage;
class UWidgetAnimation;
class UAPMPBar;
class UAPSkillSlot;
class UAPUltSkillSlot;
class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPStatusBar : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void InitStatusBar();
	void SetHPPercent(float Origin);
	void UpdatePercentBar(float InDeltaTime);
	void UpdateMPBar(uint8 Number, bool bUse = true);
	void RecoveryMP(uint8 Number);

	void IncreaseMaxMP(uint8 Number);

	void UpdateSkillSlot(ESkillKey SkillKey, uint8 SkillNumber);
	void OperateSkillSlot(ESkillKey SkillKey);
	void HightLightSkillSlot(ESkillKey SkillKey);
	void UsingSkillSlot(ESkillKey SkillKey, bool NewBool);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* HPBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UHorizontalBox* MPBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TEXT_CurrentHP;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TEXT_CurrentMaxHP;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAPSkillSlot* QSkillSlot;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAPSkillSlot* ESkillSlot;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAPUltSkillSlot* RSkillSlot;

	float OriginHP = 0.0f;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;

	TWeakObjectPtr<AAPHUD>  HUD;

	bool bChange = false;

	UPROPERTY(EditAnywhere)
	float BarSpeed = 3.0f;

	TArray<UAPMPBar*> MPSpaces;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPMPBar> MPBarClass;

	uint8 LastNum = 0;

};
