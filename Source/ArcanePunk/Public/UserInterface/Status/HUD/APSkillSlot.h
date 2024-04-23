// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Blueprint/UserWidget.h"
#include "APSkillSlot.generated.h"

class UImage;
class UTextBlock;
class UBorder;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API UAPSkillSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void UpdateSkillSlot(ESkillKey UpdateSkillKey, uint8 SkillNumber);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHightLight();
	UFUNCTION(BlueprintImplementableEvent)
	void OnOperating();
	UFUNCTION(BlueprintImplementableEvent)
	void OnCoolDown();

	void SetUsingSkill(bool NewBool);
	void StartSkillCoolTime(ESkillKey UpdateSkillKey);
	void CheckChargeTime(ESkillKey UpdateSkillKey);

private:
	void SetCoolTimePercent();
	void SetCoolTimeText();
	void SetChargeTimePercent();
    void SetChargeTimeText();

	void SkillCoolDown();
	float GetCurrentCoolTime();

	void ChargeCoolDown();
	float GetCurrentChargeTime();

	void SetSkillLimit(bool NewBool);

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

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* CoolTimePercent;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CoolTimeText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* ChargeTimePercent;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ChargeTimeText;

	float CurrentCoolTime = 0.0f;

	float SkillCoolTime = 1.0f;

	float CurrentChargeTime = 0.0f;

	float ChargeTime = 1.0f;

	bool bStartPercent = false;

	FTimerHandle SkillCoolTimerHandle;

	FTimerHandle ChargeTimerHandle;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;

	ESkillKey CurrentSkillKey;

	bool bAlreadyCharging = false;
};
