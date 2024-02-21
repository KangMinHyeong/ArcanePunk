// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneBall.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;

UCLASS()
class ARCANEPUNK_API AArcaneBall : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneBall();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	virtual void SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility) override;

	FORCEINLINE float GetCutterSpeed() const;
	void SetDeadTime(float DeadTime);

	void SetBallRadius(float Radius);
	void Explosion();

private:
	UFUNCTION()
	void BintOverlap();

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* BallTrigger;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BallMesh;
	
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* BallEffect;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* BallMoveComp;

	UPROPERTY(EditAnywhere)
	float BallSpeed = 750.0f;

	UPROPERTY(EditAnywhere)
	uint8 HitNumbers = 3;

	UPROPERTY(EditAnywhere)
	float ExplosionRadius = 250.0f;
};
