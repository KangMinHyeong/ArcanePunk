// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameElements/Volume/SpawnVolume/DataTable/APBattleStageDataTable.h"
#include "GameFramework/Actor.h"
#include "APSpawnVolume.generated.h"

class UBoxComponent;
class UAPSpawnMonsterComponent;
class AEnemy_CharacterBase;
class AAPSpawnPointBase;
class UDataTable;
class UAPBattleStageUI;

UCLASS()
class ARCANEPUNK_API AAPSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPSpawnVolume();

	FORCEINLINE FName GetSpawnDataID() const {return SpawnDataID;};

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void StartSpawn();
	void CheckNextSpawn();

	void MonsterKilled(AEnemy_CharacterBase* Enemy);
	
private:
	void InitArea();
	void InitData();
	void RegisterSpawnVolume();

	UFUNCTION()
	void SpawnMonster(FSpawnStep* SpawnStep);
	UFUNCTION()
	void RespawnMonster(FSpawnStep* SpawnStep);
	UFUNCTION()
	void DespawnMonster();

	void ActivateBattleTimerUI();

private:
	UPROPERTY(EditAnywhere)
	UAPSpawnMonsterComponent* SpawnMonsterComp;

	UPROPERTY(EditAnywhere)
	UBoxComponent* SpawnVolume;

	UPROPERTY(EditAnywhere)
	FName SpawnDataID;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPBattleStageUI> BattleUIClass;

	TWeakObjectPtr<UAPBattleStageUI> BattleUI;

	UPROPERTY(EditAnywhere)
	UDataTable* SpawnDataTable;

	FSpawnerData* SpawnData;

	TSet<AEnemy_CharacterBase*> Enemies; 

	ESpawnStepType SpawnStepType = ESpawnStepType::None;
	FName PreviousSpawnDataID;

	FVector AreaPoint_1;
	FVector AreaPoint_2;
	FVector AreaPoint_3;
	FVector AreaPoint_4;

	bool bActive = false;

	TMap<FName, FSpawnStep*> PreviousFinishSteps;
};
