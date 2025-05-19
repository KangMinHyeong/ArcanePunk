// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameElements/Trigger/BattleSection/DataTable/APBattleStageDataTable.h"
#include "GameFramework/Actor.h"
#include "APBattleSectionBase.generated.h"

class UBoxComponent;
class UAPSpawnMonsterComponent;
class AEnemy_CharacterBase;
class AAPSpawnPointBase;
class UDataTable;
class UAPBattleStageUI;

UCLASS()
class ARCANEPUNK_API AAPBattleSectionBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPBattleSectionBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE FName GetBattleSectionID() const {return BattleSectionID;};

	bool CheckSpawnEnd();

private:
	void InitArea();
	void ClearChecking();
	UFUNCTION()
	void OnSpawnLocation();
	void SpawnMonster();

private:
	UPROPERTY(EditAnywhere)
	UDataTable* SpawnDataTable;

	FBattleStageDataTable* BattleStageData;

	UPROPERTY(EditAnywhere)
	UAPSpawnMonsterComponent* SpawnMonsterComp;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BattleTrigger;

	UPROPERTY(EditAnywhere)
	uint8 SpawnNum_OnTime = 0;
	UPROPERTY(EditAnywhere)
	uint8 SpawnNum_Complete = 0;

	UPROPERTY(EditAnywhere)
	float SpawnMonsterTime = 2.5f;

	UPROPERTY(EditAnywhere)
	FName BattleSectionID;

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
