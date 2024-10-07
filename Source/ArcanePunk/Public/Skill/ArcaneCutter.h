// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneCutter.generated.h"

class UBoxComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API AArcaneCutter : public AAPSkillActorBase
{
	GENERATED_BODY()
	
public:	
	AArcaneCutter();

protected:
	virtual void BeginPlay() override;
	virtual void DestroySKill() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;
	FORCEINLINE float GetCutterSpeed() const;
	FORCEINLINE void SetDist(float Dist) { CutterDist = Dist;};
	void SetCutterWide(float Wide);

	// void SetDeadTime(float DeadTime);
	float GetTriggerWide() const;

private:
	UFUNCTION()
	void OnHitting(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	void BintHit();

	virtual void CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum) override;

	void Explosion();

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* CutterTrigger;
	
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* CutterEffect;

	TWeakObjectPtr<UNiagaraComponent> CutterEffectComp;

	UPROPERTY(EditAnywhere)
	float CutterSpeed = 1500.0f;

	UPROPERTY(EditAnywhere)
	float Drag = 6.0f;

	float CutterDist = 0.0f;

	FVector2D WidthScale;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	UProjectileMovementComponent* CutterMovementComponent;
	
	bool bExplosion = false;
	float ExplosionCoefficient = 0.0f;
	UPROPERTY(EditAnywhere)
	float ExplosionRadius = 200.0f;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ExplosionEffect;
};
