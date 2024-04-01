// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneBomb.generated.h"

class USphereComponent;
class UParticleSystem;

UCLASS()
class ARCANEPUNK_API AArcaneBomb : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneBomb();

protected:
	virtual void BeginPlay() override;	

	void InitTrigger();
	void ThrowBomb();
	void ActiveExplosion();

	UFUNCTION()
	void Explosion(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData) override;
	void SetTargetPoint(float Radius, FVector Target);

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BombMesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* BombTrigger;

	UPROPERTY(EditAnywhere)
	float InitSpeed = 1000.0f;

	UPROPERTY(EditAnywhere)
	float ThrowSpeed = 1000.0f;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere)
	FVector ExploisionScale = FVector(0,0,0);

	float ExplosionRadius = 0.0f;

	FVector TargetPoint = FVector(0,0,0);
};
