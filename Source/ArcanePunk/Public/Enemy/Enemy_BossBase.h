// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Enemy_BossBase.generated.h"


USTRUCT()
struct FRangeAttackInform
{
	GENERATED_USTRUCT_BODY()

	FRangeAttackInform() {}

	UPROPERTY(EditAnywhere)
	float Angle_Start = -60.0f;

	UPROPERTY(EditAnywhere)
	float Angle_Plus = 30.0f;

	UPROPERTY(EditAnywhere)
	float Frequency = 0.2f;

	UPROPERTY(EditAnywhere)
	int32 RepeatNum = 1;
};

UCLASS()
class ARCANEPUNK_API AEnemy_BossBase : public AEnemy_CharacterBase
{
	GENERATED_BODY()
public:
	AEnemy_BossBase();

	FORCEINLINE EBossPhase GetPhase() const {return BossPhase;};

protected:
	virtual void BeginPlay() override;

	void InitPatternNums();
	void SetRangeSpawnRot(int32 Index);

public:
	virtual void Tick(float DeltaTime) override;

	int32 GetPatternNum();

	virtual void MeleeAttack_1();
	virtual void MeleeAttack_2();

	virtual void RangeAttack_1();
	virtual void RangeAttack_2();
	virtual void RangeAttack_3();

	virtual void TraceAttack_1();
	virtual void TraceAttack_2();
	
	virtual void AroundDamage();

	virtual bool SetHPUI() override;
	
protected:
	UPROPERTY()
	EBossPhase BossPhase = EBossPhase::Phase_1;

	UPROPERTY(EditAnywhere)
	float Phase2_HPPercent = 0.6f;

	UPROPERTY(EditAnywhere)
	TMap<FString, float> Phase1_Pattern; // Pattern, Percent

	UPROPERTY(EditAnywhere)
	TMap<FString, float> Phase2_Pattern; // Pattern, Percent

	UPROPERTY(EditAnywhere)
	TArray<FRangeAttackInform> RangeAttackInform;

	TArray<int32> PatternNums;

	uint8 CurrentPatterNum = 0;

	TArray<FRotator> SpawnRot;

	FTimerHandle RangeTimerHandle;
};
