// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneCutter.generated.h"

class UBoxComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;

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

	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData) override;
	FORCEINLINE float GetCutterSpeed() const;
	void SetDeadTime(float DeadTime);
	float GetTriggerWide() const;

private:
	UFUNCTION()
	void OnHitting(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void BintHit();

private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CutterTrigger;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* CutterEffect;

	UPROPERTY(EditAnywhere)
	float CutterSpeed = 1500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	UProjectileMovementComponent* CutterMovementComponent;
	
};
