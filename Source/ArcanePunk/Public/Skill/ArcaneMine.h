// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneMine.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class ARCANEPUNK_API AArcaneMine : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneMine();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility) override;

	void Explosion(AActor* OtherActor);

private:
	UFUNCTION()
	void OnHitting(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void BintHit();

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* MineTrigger;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MineMesh;	

	UPROPERTY(EditAnywhere)
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* MineMoveComp;

	UPROPERTY(EditAnywhere)
	float MineSpeed = 750.0f;

	UPROPERTY(EditAnywhere)
	float ExplosionRadius = 250.0f;

	UPROPERTY(EditAnywhere)
	uint8 HitNumbers = 3;

	UPROPERTY(EditAnywhere)
	FVector ExplosionScale = FVector(1.0f, 1.0f, 1.0f);
};
