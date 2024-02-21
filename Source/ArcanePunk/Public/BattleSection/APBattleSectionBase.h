// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	FORCEINLINE uint8 GetTotalSpawnNumber() const {return TotalSpawnNumber;};
	FORCEINLINE uint8 GetStageNumber() const {return StageNumber;};

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
	uint8 SpawnNumber = 5;

	UPROPERTY(EditAnywhere)
	uint8 TotalSpawnNumber = 1;

	UPROPERTY()
	TArray<AActor*> SpawnLocation;

	UPROPERTY()
	TArray<AEnemy_CharacterBase*> MonsterArr;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AAPSpawnPointBase>> SpawnPointClasses;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AEnemy_CharacterBase>> SpawnMonsterClasses;

	FTimerHandle SpawnMonsterTimerHandle;

	UPROPERTY(EditAnywhere)
	float SpawnMonsterTime = 2.5f;

	UPROPERTY(EditAnywhere)
	uint8 StageNumber = 1;
	
};
