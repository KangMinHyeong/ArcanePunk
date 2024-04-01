// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/Status/HUD/APSkillSlot.h"
#include "APUltSkillSlot.generated.h"

class UBorder;
class UTextBlock;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API UAPUltSkillSlot : public UAPSkillSlot
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void UpdateSkillSlot(uint8 SkillNumber) override;
	
	void StartSkillCoolTime();
	UFUNCTION(BlueprintImplementableEvent)
	void OnCoolDown();
	
private:
	void SetCoolTimePercent();
	void SetCoolTimeText();

	void SkillCoolDown();
	float GetCurrentCoolTime();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* CoolTimePercent;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CoolTimeText;

	UPROPERTY(EditAnywhere)
	float CurrentCoolTime = 0.0f;

	UPROPERTY(EditAnywhere)
	float SkillCoolTime = 1.0f;

	bool bStartPercent = false;

	FTimerHandle SkillCoolTimerHandle;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
};
