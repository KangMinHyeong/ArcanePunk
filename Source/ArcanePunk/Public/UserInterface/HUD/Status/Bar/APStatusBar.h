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
class UAPDashSlot;

UCLASS()
class ARCANEPUNK_API UAPStatusBar : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	void InitStatusBar();
	
	UFUNCTION()
	void SetMaxHP(float NewValue);
	UFUNCTION()
	void SetHPPercent(float Origin);
	
	UFUNCTION()
	void AddMaxMP(uint8 NewValue);
	UFUNCTION()
	void UpdateMPBar(uint8 Number, bool bUse = true);

	UFUNCTION()
	void IncreaseMaxMP(uint8 Number);

	UFUNCTION()
	void UpdateSkillSlot(ESkillKey SkillKey, uint8 SkillNumber);
	UFUNCTION()
	void OperateSkillSlot(ESkillKey SkillKey);
	UFUNCTION()
	void HightLightSkillSlot(ESkillKey SkillKey);
	UFUNCTION()
	void UsingSkillSlot(ESkillKey SkillKey, bool NewBool);
	UFUNCTION()
	void StartCoolTimeSlot(ESkillKey SkillKey, float CoolTime);
	UFUNCTION()
	void CheckChargeTime(ESkillKey SkillKey);
	UFUNCTION()
	void ChangeCharginNum(ESkillKey SkillKey);
	UFUNCTION()
	void ChargeGauge(float Currnet, float Max);
	UFUNCTION()
	void ChargeEnd(bool Start);
	UFUNCTION()
	void AddSkillCoolTime(ESkillKey SkillKey, float AddTime);
	UFUNCTION()
	void StartDashTime(float DashTime);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAPHPBar* APHPBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UHorizontalBox* MPBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAPSkillSlot* Slot_QSkill;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAPSkillSlot* Slot_ESkill;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAPSkillSlot* Slot_RSkill;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAPDashSlot* Slot_Dash;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAPSkillSlot* Slot_Parrying;

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
