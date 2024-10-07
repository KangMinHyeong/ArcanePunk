// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneField.generated.h"

class USphereComponent;
class UDecalComponent;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API AArcaneField : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneField();

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
	void FollowCharacter(float DeltaTime);

	void SetFieldDebuff();
	void SetFieldDamage();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);
	void BindOverlap();

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* FieldTrigger;	

	UPROPERTY(EditAnywhere)
	UDecalComponent* FieldDecal;	

	UPROPERTY(EditAnywhere)
	float FollowSpeed = 3.0f;

	FTimerHandle FieldTimerHandle;
	FTimerHandle DamageTimerHandle;
	
	UPROPERTY(EditAnywhere)
	float DebuffRateTime = 1.25f;

	UPROPERTY(EditAnywhere)
	float DamageRateTime = 3.0f;

	TWeakObjectPtr<AArcanePunkCharacter> Character;

	float FieldDamage;
};
