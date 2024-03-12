// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcanePunk/APGameModeBase.h"
#include "APGameModeBattleStage.generated.h"

class UDataTable;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API AAPGameModeBattleStage : public AAPGameModeBase
{
	GENERATED_BODY()

public:
	void MonsterKilled();
	FORCEINLINE UDataTable* GetBattleStageDataTable() const {return BattleStageDataTable;};
	FORCEINLINE UNiagaraSystem* GetSpawnEffect() const {return SpawnEffect;}; 

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

};
