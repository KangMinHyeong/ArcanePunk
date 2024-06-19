// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "APTitleGameMode.generated.h"

UCLASS()
class ARCANEPUNK_API AAPTitleGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AAPTitleGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;	
	
	virtual void StartPlay() override;

	UFUNCTION()
	void OnChangingBGMVolume(float Master, float BGM, float Effect);
	
private:
	UPROPERTY(EditAnywhere)
	USoundBase* BGM_Cue;

	TWeakObjectPtr<UAudioComponent> GameMode_BGM;
};
