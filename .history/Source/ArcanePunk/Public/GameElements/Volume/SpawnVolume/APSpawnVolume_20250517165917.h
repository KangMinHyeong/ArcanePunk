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

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE FName GetSpawnDataID() const {return SpawnDataID;};

	bool CheckSpawnEnd();

private:
	void InitArea();
	void InitData();
	void RegisterSpawner();

	UFUNCTION()
	void OnSpawnLocation();
	void SpawnMonster();

private:
	UPROPERTY(EditAnywhere)
	UDataTable* SpawnDataTable;

	FSpawnerData* SpawnData;

	UPROPERTY(EditAnywhere)
	UAPSpawnMonsterComponent* SpawnMonsterComp;

	UPROPERTY(EditAnywhere)
	UBoxComponent* SpawnVolume;

	uint8 SpawnStepCount = 0;

	UPROPERTY(EditAnywhere)
	float SpawnMonsterTime = 2.5f;

	UPROPERTY(EditAnywhere)
	FName SpawnDataID;

	FVector AreaPoint_1;
	FVector AreaPoint_2;
	FVector AreaPoint_3;
	FVector AreaPoint_4;

	bool bOvelapped = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPBattleStageUI> BattleUIClass;

	TWeakObjectPtr<UAPBattleStageUI> BattleUI;

	FTimerHandle TriggerTimerHandle;
};
