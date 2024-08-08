// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/APGameModeBase.h"
#include "APGameModeBattleStage.generated.h"

class UDataTable;
class UNiagaraSystem;
class USoundBase;

DECLARE_MULTICAST_DELEGATE(FOnMonsterKilled);

UCLASS()
class ARCANEPUNK_API AAPGameModeBattleStage : public AAPGameModeBase
{
	GENERATED_BODY()

public:
	void MonsterKilled();
	void PlayerKilled();
	FORCEINLINE UDataTable* GetBattleStageDataTable() const {return BattleStageDataTable;};
	FORCEINLINE UNiagaraSystem* GetSpawnEffect() const {return SpawnEffect;}; 
	FORCEINLINE USoundBase* GetSpawnSound() const {return SpawnSound;}; 
	FORCEINLINE float GetSpawnSoundVolume() const {return SpawnSoundVolume;}; 

protected:
	virtual void StartPlay() override;

private:
	void EndBattleSection();
	void PortalSpawn();

private:
	FTimerHandle PortalSpawnTimerHandle;

	UPROPERTY()
	FName CurrentClearStage;

	UPROPERTY(EditAnywhere)
	UDataTable* BattleStageDataTable;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* SpawnEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* SpawnSound;

	UPROPERTY(EditAnywhere)
	float SpawnSoundVolume = 2.5f;

public:
	FOnMonsterKilled OnMonsterKilled;
};
