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
	virtual void SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility) override;	
	
	virtual void DestroySKill() override;	

private:
	void FollowCharacter(float DeltaTime);

	void SetFieldDebuff();
	void SetFieldDamage();

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

	UPROPERTY()
	AArcanePunkCharacter* Character;

	UPROPERTY(EditAnywhere)
	float SlowCoefficient = 0.5f;
};
