// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Install/APInstallBase.h"
#include "APTurret.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API AAPTurret : public AAPInstallBase
{
	GENERATED_BODY()
public:	
	AAPTurret();

protected:
	virtual void BeginPlay() override;	

	virtual void DestroySKill() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void InitInstall(ASkillActor* SkillActor);
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
private:
	void AutoAttack();
	AActor* FindEnemy();
	void Fire();

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RotateMesh;
};
