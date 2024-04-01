// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "SwordThrowBase.generated.h"

UCLASS()
class ARCANEPUNK_API ASwordThrowBase : public AAPSkillActorBase
{
	GENERATED_BODY()
	
public:	
	ASwordThrowBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnHitting(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData) override;

protected:
	void ScaleSet();

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Sword;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* TrailEffect;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* HitTrigger;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* HitEffect;

	UPROPERTY(EditAnywhere)
	float SwordSpeed = 1500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	class UProjectileMovementComponent* SwordMovementComponent;
	
};
