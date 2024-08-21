#pragma once

#include "CoreMinimal.h"
#include "GameElements/Trigger/BattleSection/DataTable/APBattleStageDataTable.h"
#include "GameFramework/GameModeBase.h"
#include "APGameModeBase.generated.h"

UCLASS()
class ARCANEPUNK_API AAPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAPGameModeBase();
	virtual void PostLogin(APlayerController* NewPlayer) override;	
	FORCEINLINE bool IsBattleStage() const {return bBattleStage;};

	UFUNCTION()
	void OnChangingBGMVolume(float Master, float BGM, float Effect);

protected:
	void SpawnMonster();
	virtual void StartPlay() override;

protected:
	FTimerHandle MonsterSpawnTimerHandle;

	float SpawnInterval = 30.0f;

	UPROPERTY(EditAnywhere)
	bool bBattleStage = false;

	UPROPERTY(EditAnywhere)
	USoundBase* BGM_Cue;
	TWeakObjectPtr<UAudioComponent> GameMode_BGM;
};
