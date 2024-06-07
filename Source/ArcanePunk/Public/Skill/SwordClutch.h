// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "SwordClutch.generated.h"

class AArcanePunkCharacter;
class AClutchTrigger;

UCLASS()
class ARCANEPUNK_API ASwordClutch : public AAPSkillActorBase
{
	GENERATED_BODY()
public:
	ASwordClutch();

protected:
	virtual void BeginPlay() override;	
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;
	virtual void CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum) override;
		
	virtual void DestroySKill() override;	

	FORCEINLINE void SetDistance(float NewValue) {Distance = NewValue;};
	FORCEINLINE void SetWide(float NewValue) {Wide = NewValue;};
	FORCEINLINE void SetClutchSpeed(float NewValue) {ClutchSpeed = ClutchSpeed * NewValue;};
	FORCEINLINE void AddDestroyTime(float Add) {DestroyTime = DestroyTime + Add;};
	
	FORCEINLINE bool IsDrain() const {return bDrain;};
	FORCEINLINE float GetDrainCoefficient() const {return DrainCoefficient;};
	FORCEINLINE bool IsDurationUpgrade() const {return bDurationUpgrade;};
	FORCEINLINE float GetDurationUpgradeCoefficient() const {return DurationUpgradeCoefficient;};
	
private:
	void SpawnClutchTrigger();

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* ClutchRoot;

	float Distance = 0.0f;

	float Wide = 0.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AClutchTrigger> ClutchTriggerClass;

	UPROPERTY(EditAnywhere)
	float ClutchSpeed = 1.0f;

	int32 SwordNumber = 4;

	bool bDrain = false;

	float DrainCoefficient = 0.0f;

	bool bDurationUpgrade = false;
	float DurationUpgradeCoefficient = 0.0f;
};
