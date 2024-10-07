// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneTent.generated.h"

class USphereComponent;
class UDecalComponent;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API AArcaneTent : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneTent();

protected:
	virtual void BeginPlay() override;	
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;
	virtual void CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum) override;

	virtual void DestroySKill() override;	

private:
	UFUNCTION()
  	void OnBeginHiding(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndHiding(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetDotDamage();
	void CheckOverlap(bool NewBool);

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* TentTrigger;	

	UPROPERTY(EditAnywhere)
	UDecalComponent* TentDecal;		

	UPROPERTY(EditAnywhere)
	float ReturnToHideTime = 0.5f;

	FTimerHandle DotRateTimerHandle;

	UPROPERTY(EditAnywhere)
	float DotRateTime = 3.5f;

	float DotPercent = 0.0f;

	bool bFollowing = false;

	float FollowingSpeed = 0.0f;

	bool bDrainField = false;

	float TentFieldDrainCoefficient = 0.0f;
};
