// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcanePunk/APGameModeBase.h"
#include "APGameModeBattleStage.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API AAPGameModeBattleStage : public AAPGameModeBase
{
	GENERATED_BODY()

public:
	void MonsterKilled();

protected:
	virtual void StartPlay() override;
	
private:
	void EndBattleSection();
	void PortalSpawn();

private:
	FTimerHandle PortalSpawnTimerHandle;

	UPROPERTY()
	uint8 CurrentClearStage = 0;

};
