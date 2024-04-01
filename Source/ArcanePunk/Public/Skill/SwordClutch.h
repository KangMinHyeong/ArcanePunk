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
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData) override;
	
	virtual void DestroySKill() override;	

	FORCEINLINE void SetDistance(float NewValue) {Distance = NewValue;};
	FORCEINLINE void SetWide(float NewValue) {Wide = NewValue;};
	FORCEINLINE void SetClutchSpeed(float NewValue) {ClutchSpeed = ClutchSpeed * NewValue; UE_LOG(LogTemp, Display, TEXT("Your %f"), NewValue);};

private:
	void SpawnClutchTrigger();

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* ClutchRoot;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;	

	float Distance = 0.0f;

	float Wide = 0.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AClutchTrigger> ClutchTriggerClass;

	UPROPERTY(EditAnywhere)
	float ClutchSpeed = 1.0f;
};
