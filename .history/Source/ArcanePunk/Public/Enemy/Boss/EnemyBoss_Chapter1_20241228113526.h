// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy_BossBase.h"
#include "EnemyBoss_Chapter1.generated.h"

UCLASS()
class ARCANEPUNK_API AEnemyBoss_Chapter1 : public AEnemy_BossBase
{
	GENERATED_BODY()
public:
	AEnemyBoss_Chapter1();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual void MeleeAttack_1() override;
	virtual void MeleeAttack_2() override;

	virtual void RangeAttack_1() override;
	virtual void RangeAttack_2() override;
	virtual void RangeAttack_3() override;

	virtual void TraceAttack_1() override;
	virtual void TraceAttack_2() override;

	virtual void AroundDamage() override;

	virtual void SpawnAttackRange(AActor* Target, float WaitTime) override;

	virtual bool IsDead() override;

	UFUNCTION()
	void OnOverlapping(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	virtual void RemoveHPUI() override;
	
private:
	UPROPERTY(EditAnywhere)
	USceneComponent* AmmoSpawnComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> AmmoClass;

	UPROPERTY(EditAnywhere, Category = "Range")
	float Monster_RangeAttack_Range = 1200.0f;
	UPROPERTY(EditAnywhere, Category = "Range")
	float Monster_RangeAttack_Radius = 60.0f;

	float Monster_TraceAttack1_Range = 150.0f;
	bool bTraceAttack = false;
	FVector TargetLocation;
	UPROPERTY(EditAnywhere, Category = "Range")
	float TraceSpeed = 5.0f;

	bool bJump = false;
	UPROPERTY(EditAnywhere, Category = "Range")
	float Monster_JumpAttack_Range = 400.0f;

	FTimerHandle EndTimerHandle;
};
