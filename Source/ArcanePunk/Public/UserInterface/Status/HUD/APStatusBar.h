// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Blueprint/UserWidget.h"
#include "APStatusBar.generated.h"

class AAPHUD;
class AArcanePunkCharacter;
class UHorizontalBox;
class UImage;
class UWidgetAnimation;
class UAPMPBar;
class UAPSkillSlot;
class UAPUltSkillSlot;
class UAPHPBar;
class UAPSkillChargingGauge;

UCLASS()
class ARCANEPUNK_API UAPStatusBar : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	void InitStatusBar();
	
	void SetHPPercent(float Origin);

	void UpdateMPBar(uint8 Number, bool bUse = true);
	void RecoveryMP(uint8 Number);
	void IncreaseMaxMP(uint8 Number);

	void UpdateSkillSlot(ESkillKey SkillKey, uint8 SkillNumber);
	void OperateSkillSlot(ESkillKey SkillKey);
	void HightLightSkillSlot(ESkillKey SkillKey);
	void UsingSkillSlot(ESkillKey SkillKey, bool NewBool);
	void StartCoolTimeSlot(ESkillKey SkillKey);
	void CheckChargeTime(ESkillKey SkillKey);
	void ChargeGauge(float Currnet, float Max);
	void ChargeEnd(bool Start);
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAPHPBar* APHPBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UHorizontalBox* MPBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAPSkillSlot* QSkillSlot;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAPSkillSlot* ESkillSlot;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAPUltSkillSlot* RSkillSlot;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAPSkillChargingGauge* SkillChargingGauge;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;

	TWeakObjectPtr<AAPHUD>  HUD;

	TArray<UAPMPBar*> MPSpaces;
	TArray<UAPMPBar*> Remain_MPBars;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPMPBar> MPBarClass;

	uint8 LastNum = 0;

};
