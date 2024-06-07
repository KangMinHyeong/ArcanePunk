// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "MoonSlash.generated.h"

class UBoxComponent;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API AMoonSlash : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AMoonSlash();

protected:
	virtual void BeginPlay() override;	

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;
	FORCEINLINE void SetMoonSlashWidth(float Width) {MoonSlashWidth = Width;};
	FORCEINLINE void SetMoonSlashAngle(float Angle) {MoonSlashAngle = Angle;};

private:
	virtual void DestroySKill() override;
	void ApplyMoonSlashSetting();

	UFUNCTION()
	void OnSlashDamage(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* MoonSlashRoot;

	UPROPERTY(EditAnywhere)
	UBoxComponent* MoonSlashTrigger;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* MoonSlashEffect;

	UPROPERTY(EditAnywhere)
	float MoonSlashSpeed = 150.0f;
	
	float MoonSlashWidth;
	float MoonSlashAngle;

	FRotator CurrentRot;
	FRotator TargetRot;
	UPROPERTY(EditAnywhere)
	FRotator PlusEffectRot;
};
