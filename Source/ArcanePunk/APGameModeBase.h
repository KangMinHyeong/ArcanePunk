#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "APGameModeBase.generated.h"

UCLASS()
class ARCANEPUNK_API AAPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAPGameModeBase();
	virtual void PostLogin(APlayerController* NewPlayer) override;	

protected:
	void SpawnMonster();

	FTimerHandle MonsterSpawnTimerHandle;

	float SpawnInterval = 30.0f;

	virtual void StartPlay() override;
};
