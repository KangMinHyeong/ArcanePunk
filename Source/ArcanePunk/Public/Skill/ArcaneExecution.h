// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneExecution.generated.h"

class USphereComponent;
class ACharacter;
class UNiagaraComponent;
class UNiagaraSystem;

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	None		 = 0,
    First		 = 1,
    Second	     = 2,
};

UCLASS()
class ARCANEPUNK_API AArcaneExecution : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneExecution();

protected:
	virtual void BeginPlay() override;	
	
	void Attack_First();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;

	FORCEINLINE void SetTargetEnemy(ACharacter* Enemy) {TargetEnemy = Enemy;};
	FORCEINLINE void SetTargetEnemyLocation(FVector Location) {TargetEnemyLocation = Location;};
	void SetExecutionRadius(float Radius);
	

	virtual void DestroySKill() override;
	
	void SetExecutionType(uint8 NewValue);
	
private:	
	UPROPERTY(EditAnywhere)
	USphereComponent* ExecutionSphere;

	UPROPERTY(EditAnywhere)
	float FirstAttackSpeed = 1500.0f;

	UPROPERTY(EditAnywhere)
	float SecondAttackSpeed = 2500.0f;

	UPROPERTY(EditAnywhere)
	float PlusDist = 150.0f;

	EAttackType AttackType = EAttackType::None;

	FVector TargetEnemyLocation;

	ACharacter* TargetEnemy;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* FirstEffect;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* SecondEffect;
};
