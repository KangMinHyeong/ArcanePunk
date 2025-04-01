// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SkillDataTable/SkillDataTable.h"
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

	void BindStatusBar(AArcanePunkCharacter* OwnerPlayer);
	
	void SetMaxHP(float NewValue);
	void SetHPPercent(float Origin);

	void AddMaxMP(uint8 NewValue);
	void UpdateMPBar(uint8 Number, bool bUse = true);
	void IncreaseMaxMP(uint8 Number);

	void UpdateSkillSlotImage(ESkillKey SkillKey, UTexture2D* SkillSlotImage);
	void UpdateSkillSlot(AArcanePunkCharacter* Character, ESkillKey SkillKey);
	void OperateSkillSlot(ESkillKey SkillKey);
	void HightLightSkillSlot(ESkillKey SkillKey);
	void UsingSkillSlot(ESkillKey SkillKey, bool NewBool);
	void StartCoolTimeSlot(ESkillKey SkillKey, float CoolTime);
	void CheckChargeTime(ESkillKey SkillKey);
	void ChangeCharginNum(ESkillKey SkillKey);
	void ChargeGauge(float Currnet, float Max);
	void ChargeEnd(bool Start);
	void AddSkillCoolTime(ESkillKey SkillKey, float AddTime);
	void StartDashTime(float DashTime);
	
	void SetHiddenGuideBar(bool bVisible);

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

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* GuideBar;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;

	TWeakObjectPtr<AAPHUD>  HUD;

	TArray<UAPMPBar*> MPSpaces;
	TArray<UAPMPBar*> Remain_MPBars;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPMPBar> MPBarClass;

	uint8 LastNum = 0;

};
