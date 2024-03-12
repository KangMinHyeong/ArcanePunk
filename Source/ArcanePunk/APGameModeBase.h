#pragma once

#include "CoreMinimal.h"
#include "BattleSection/DataTable/APBattleStageDataTable.h"
#include "GameFramework/GameModeBase.h"
#include "APGameModeBase.generated.h"

struct FBattleStageInformation;

UCLASS()
class ARCANEPUNK_API AAPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAPGameModeBase();
	virtual void PostLogin(APlayerController* NewPlayer) override;	
	FORCEINLINE bool IsBattleStage() const {return bBattleStage;};
	FORCEINLINE FBattleStageInformation GetBattleStageInformation() const {return BattleStageInformation;};
	FORCEINLINE void SetBattleStageInformation(FBattleStageInformation NewBattleStageInformation) {BattleStageInformation = NewBattleStageInformation;};

	FORCEINLINE uint8 GetTotalMonsterNumber() const {return TotalMonsterNumber;};
	FORCEINLINE void SetTotalMonsterNumber(uint8 NewTotalMonsterNumber) {TotalMonsterNumber = NewTotalMonsterNumber;};
	FORCEINLINE uint8 GetRemainMonsterNumber() const {return RemainMonsterNumber;};
	FORCEINLINE void SetRemainMonsterNumber(uint8 NewRemainMonsterNumber) {RemainMonsterNumber = NewRemainMonsterNumber;};


protected:
	void SpawnMonster();
	virtual void StartPlay() override;

protected:
	FTimerHandle MonsterSpawnTimerHandle;

	float SpawnInterval = 30.0f;

	UPROPERTY(EditAnywhere)
	bool bBattleStage = false;

	UPROPERTY()
	FBattleStageInformation BattleStageInformation;

	UPROPERTY(EditAnywhere)
	uint8 TotalMonsterNumber = 0;

	UPROPERTY(EditAnywhere)
	uint8 RemainMonsterNumber = 0;
};
