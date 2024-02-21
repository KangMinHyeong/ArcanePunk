// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerState/APPlayerData.h"
#include "GameFramework/PlayerState.h"
#include "ArcanePunkPlayerState.generated.h"


UCLASS()
class ARCANEPUNK_API AArcanePunkPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AArcanePunkPlayerState();
	void SavePlayerData();
	void InitPlayerData();
	void UpdatePlayerData(FPlayerTotalData& PS);
	void DeathPenalty();

public:
	UPROPERTY(Transient, BlueprintReadWrite)
	FPlayerTotalData PlayerTotalStatus = {};
	
};
