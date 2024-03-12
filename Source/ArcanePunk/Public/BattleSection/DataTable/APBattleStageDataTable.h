// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "APBattleStageDataTable.generated.h"

class AEnemy_CharacterBase;

USTRUCT()
struct FBattleStageInformation
{
	GENERATED_USTRUCT_BODY()

	FBattleStageInformation() {}

	UPROPERTY(EditAnywhere, meta = (MultiLine = true))
	FString StageTitle;

	UPROPERTY(EditAnywhere)
	uint8 ChapterNumber = 0;

	UPROPERTY(EditAnywhere, meta = (MultiLine = true))
	FString StageClearCondition;
};

USTRUCT()
struct FSpawnMonsterData
{
	GENERATED_USTRUCT_BODY()

	FSpawnMonsterData() {}

	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AEnemy_CharacterBase>, uint8> SpawnMonsterType;
};

USTRUCT()
struct FBattleStageDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FBattleStageDataTable() {}

	UPROPERTY(EditAnywhere)
	FName BattleSectionID;

	UPROPERTY(EditAnywhere)
	FBattleStageInformation BattleStageInformation;

	UPROPERTY(EditAnywhere)
	TArray<FSpawnMonsterData> SpawnMonsterData;
};

UCLASS()
class ARCANEPUNK_API UAPBattleStageDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
