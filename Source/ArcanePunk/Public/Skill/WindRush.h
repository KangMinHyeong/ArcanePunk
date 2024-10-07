// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "WindRush.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UBoxComponent;

UCLASS()
class ARCANEPUNK_API AWindRush : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AWindRush();

protected:
	virtual void BeginPlay() override;	

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;

	void SetWindRushWidth(float Width);
	void SetTargetLocation(const FVector & Location, float Dist);
	// void SetExecutionRadius(float Radius);
	

	virtual void DestroySKill() override;

private:	
	UPROPERTY(EditAnywhere)
	UBoxComponent* WindRushTrigger;	

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* WindRushEffect;

	UPROPERTY()
	UNiagaraComponent* RushEffectComp;
	
	UPROPERTY(EditAnywhere)
	float RushSpeed = 1750.0f;

	FVector TargetLocation;

	UPROPERTY(EditAnywhere)
	float BleedingDamage = 5.0f;

	UPROPERTY(EditAnywhere)
	float BleedingRate = 3.0f;
};
