// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ShadowShuriken.generated.h"

class AClutchTrigger;

UCLASS()
class ARCANEPUNK_API AShadowShuriken : public AAPSkillActorBase
{
	GENERATED_BODY()
public:
	AShadowShuriken();

protected:
	virtual void BeginPlay() override;	
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;
		
	virtual void DestroySKill() override;	
	
	void CollectShuriken();
private:
	void SpawnShuriken();
	void SpreadShuriken(float DeltaTime);
	void CollectShuriken(float DeltaTime);

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* ShurikenRoot;

	UPROPERTY(EditAnywhere)
	float ShurikenDist = 600.0f;

	UPROPERTY(EditAnywhere)
	float ShurikenWidth = 50.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AClutchTrigger> ShurikenClass;
	
	UPROPERTY()
	TArray<AClutchTrigger*> Shurikens;

	UPROPERTY()
	TArray<FVector> ShurikenTargetLocation;

	UPROPERTY()
	TArray<float> ShurikenCollectSpeed;

	UPROPERTY(EditAnywhere)
	float ShurikenSpeed = 500.0f;

	UPROPERTY(EditAnywhere)
	float CollectSpeedCoefficient = 1.5f;

	UPROPERTY(EditAnywhere)
	int32 ShurikenNumber = 4;

	bool bComplete = false;
	
	FTimerHandle CollectTimerHandle;

	UPROPERTY(EditAnywhere)
	float CollectTime = 5.0f;

};
