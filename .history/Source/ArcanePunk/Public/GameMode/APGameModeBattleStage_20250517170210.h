// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/APGameModeBase.h"
#include "APGameModeBattleStage.generated.h"

class UNiagaraSystem;
class USoundBase;

UCLASS()
class ARCANEPUNK_API AAPGameModeBattleStage : public AAPGameModeBase
{
	GENERATED_BODY()

public:
	void MonsterKilled(AActor* SpawnVolume, bool ff);
	void PlayerKilled();
	FORCEINLINE UNiagaraSystem* GetSpawnEffect() const {return SpawnEffect;}; 
	FORCEINLINE USoundBase* GetSpawnSound() const {return SpawnSound;}; 
	FORCEINLINE float GetSpawnSoundVolume() const {return SpawnSoundVolume;}; 

	void OnEndedGame();
	
protected:
	virtual void StartPlay() override;

private:
	void CheckBattleSection(AActor* SpawnVolume);
	void PortalSpawn(FName CurrentClearStage);

private:
	FTimerHandle PortalSpawnTimerHandle;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* SpawnEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* SpawnSound;

	UPROPERTY(EditAnywhere)
	float SpawnSoundVolume = 2.5f;

};
