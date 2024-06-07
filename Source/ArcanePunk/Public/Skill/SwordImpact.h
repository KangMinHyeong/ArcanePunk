// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "SwordImpact.generated.h"

class UBoxComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API ASwordImpact : public AAPSkillActorBase
{
	GENERATED_BODY()
	
public:	
	ASwordImpact();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHitting(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
  	void OnPenetrating(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;

private:
	UFUNCTION()
	void BintHit();
	void DamageAction(AActor *OtherActor, const FHitResult &HitResult);
	void SlowPlayer(AActor *OtherActor);

private:
	UPROPERTY(EditAnywhere)
	bool IsPenetrate = false;

	UPROPERTY(EditAnywhere)
	UBoxComponent* ImpactComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta=(AllowPrivateAccess="true"))
	UNiagaraComponent* BaseEffect;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* HitEffect;

	UPROPERTY(EditAnywhere)
	float ImpactSpeed = 1500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	UProjectileMovementComponent* ImpactMovementComponent;
	
	UPROPERTY(EditAnywhere)
	bool IsPlayerSkill = true;


};
