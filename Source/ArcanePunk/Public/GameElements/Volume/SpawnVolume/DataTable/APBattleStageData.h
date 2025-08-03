// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "APBattleStageData.generated.h"

class AEnemy_CharacterBase;

USTRUCT()
struct FBattleWaveData
{
	GENERATED_USTRUCT_BODY()

	FBattleWaveData() {}

	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AEnemy_CharacterBase>, uint8> SpawnMonsterData;
};

USTRUCT()
struct FBattleStageData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FBattleStageData() {}

	UPROPERTY(EditAnywhere)
	FName BattleSectionID;

	UPROPERTY(EditAnywhere)
	int32 Chapter = 0;

	UPROPERTY(EditAnywhere)
	float NextWaveTime = 15.0f;

	UPROPERTY(EditAnywhere)
	TArray<FBattleWaveData> Spawn_OnTime; // On Spawning Monster In Loop

	UPROPERTY(EditAnywhere)
	TArray<FBattleWaveData> Spawn_Complete; // On Killed All Spawn_OnTime Monster
};

UENUM(BlueprintType)
enum class ESpawnStepStartCondition : uint8 // 스포너 스탭이 시작되는 조건 
{
	None		 					= 0 UMETA(DisplayName = "None"),
	SpawnerActive				 	= 1 UMETA(DisplayName = "SpawnerActive"),
	PreviousSpawnStepFinish		 	= 2 UMETA(DisplayName = "PreviousSpawnStepFinish"),
};

UENUM(BlueprintType)
enum class ESpawnStepType : uint8 // 스포너 스탭의 타입 
{
	None		 	= 0 UMETA(DisplayName = "None"),
	Basic			= 1 UMETA(DisplayName = "Basic"),
	Timer		 	= 2 UMETA(DisplayName = "Timer"),
};

USTRUCT(BlueprintType)
struct FSpawnEntryData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TSubclassOf<AEnemy_CharacterBase> SpawnEntity;

    UPROPERTY(EditAnywhere)
    uint8 EntityQuantity;
};

USTRUCT(BlueprintType)
struct FSpawnStepData
{
	GENERATED_USTRUCT_BODY()

	FSpawnStepData() {}

	UPROPERTY(EditAnywhere)
	FName SpawnStepID;

	UPROPERTY(EditAnywhere)
	ESpawnStepStartCondition SpawnStepStartCondition;

	UPROPERTY(EditAnywhere, meta=(EditCondition="SpawnStepStartCondition == ESpawnStepStartCondition::PreviousSpawnStepFinish"))
	FName PreviousSpawnStepID;

	UPROPERTY(EditAnywhere)
	ESpawnStepType SpawnStepType;

	UPROPERTY(EditAnywhere, meta=(EditCondition="SpawnStepType == ESpawnStepType::Timer"))
	float TimerValue = 0.0f;
	
	UPROPERTY(EditAnywhere)
	TArray<FSpawnEntryData> SpawnEntry;
};

UCLASS()
class ARCANEPUNK_API UAPBattleStageData : public UDataTable
{
	GENERATED_BODY()
	
};
