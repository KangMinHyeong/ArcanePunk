// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleSection/DataTable/APBattleStageDataTable.h"
#include "GameFramework/Actor.h"
#include "APBattleSectionBase.generated.h"

class UBoxComponent;
class UAPSpawnMonsterComponent;
class AEnemy_CharacterBase;
class AAPSpawnPointBase;

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	FORCEINLINE uint8 GetTotalSpawnNumber() const {return TotalSpawnNum;};
	FORCEINLINE FName GetBattleSectionID() const {return BattleSectionID;};
	FORCEINLINE USceneComponent* GetSpawnRoot() const {return SpawnRoot;};
	FORCEINLINE USceneComponent* GetSpawnRoot2() const {return SpawnRoot2;};

	bool CheckSpawnEnd();

private:
	void ClearChecking();
	UFUNCTION()
	void OnSpawnLocation(UPrimitiveComponent*OverlappedComp, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void SpawnMonster();

private:
	UPROPERTY(EditAnywhere)
	UAPSpawnMonsterComponent* SpawnMonsterComp;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BattleTrigger;

	UPROPERTY(EditAnywhere)
	USceneComponent* SpawnRoot;

	UPROPERTY(EditAnywhere)
	USceneComponent* SpawnRoot2;

	UPROPERTY(EditAnywhere)
	uint8 TotalSpawnNum = 0;

	FTimerHandle SpawnMonsterTimerHandle;

	UPROPERTY(EditAnywhere)
	float SpawnMonsterTime = 2.5f;

	UPROPERTY(EditAnywhere)
	FName BattleSectionID;


	// UPROPERTY(EditAnywhere)
	// uint8 SpawnNumber = 5;

	// UPROPERTY()
	// TArray<AActor*> SpawnLocation;

	// UPROPERTY()
	// TArray<AEnemy_CharacterBase*> MonsterArr;

	// UPROPERTY(EditAnywhere)
	// TArray<TSubclassOf<AAPSpawnPointBase>> SpawnPointClasses;

	// UPROPERTY(EditAnywhere)
	// TArray<TSubclassOf<AEnemy_CharacterBase>> SpawnMonsterClasses;
	
};
