#pragma once

#include "CoreMinimal.h"
#include "GameElements/Trigger/BattleSection/DataTable/APBattleStageDataTable.h"
#include "GameFramework/GameModeBase.h"
#include "APGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndedConversation, int32, GroupID);

UCLASS()
class ARCANEPUNK_API AAPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAPGameModeBase();
	virtual void PostLogin(APlayerController* NewPlayer) override;	
	FORCEINLINE bool IsBattleStage() const {return bBattleStage;};

	UFUNCTION()
	void OnChangingBGMVolume(float Master, float BGM, float Effect, float UI);

protected:
	virtual void StartPlay() override;

	void SpawnPlayers();

protected:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APawn>> PlayerClass;
	UPROPERTY()
	TArray<APawn*> Players;

	UPROPERTY(EditAnywhere)
	bool bBattleStage = false;

	UPROPERTY(EditAnywhere)
	USoundBase* BGM_Cue;
	TWeakObjectPtr<UAudioComponent> GameMode_BGM;

public:
	FOnEndedConversation OnEndedConversation;

	UPROPERTY()
	uint8 MainPlayerIndex = 0;
};
