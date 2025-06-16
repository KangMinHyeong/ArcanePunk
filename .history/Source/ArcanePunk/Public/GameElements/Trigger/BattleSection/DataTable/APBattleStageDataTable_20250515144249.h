 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "APBattleStageDataTable.generated.h"

class AEnemy_CharacterBase;

USTRUCT()
struct FBattleWaveDataTable
{
	GENERATED_USTRUCT_BODY()

	FBattleWaveDataTable() {}

	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AEnemy_CharacterBase>, uint8> SpawnMonsterData;
};

USTRUCT()
struct FBattleStageDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FBattleStageDataTable() {}

	UPROPERTY(EditAnywhere)
	FName BattleSectionID;

	UPROPERTY(EditAnywhere)
	int32 Chapter = 0;

	UPROPERTY(EditAnywhere)
	float NextWaveTime = 15.0f;

	UPROPERTY(EditAnywhere)
	TArray<FBattleWaveDataTable> Spawn_OnTime; // On Spawning Monster In Loop

	UPROPERTY(EditAnywhere)
	TArray<FBattleWaveDataTable> Spawn_Complete; // On Killed All Spawn_OnTime Monster
};

USTRUCT()
struct FSpawnerData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FBattleStageDataTable() {}

	UPROPERTY(EditAnywhere)
	FName SpawnerID;

	
};


UCLASS()
class ARCANEPUNK_API UAPBattleStageDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
