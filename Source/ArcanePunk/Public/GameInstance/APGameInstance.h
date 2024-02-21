// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "APGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UAPGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

public:
	UPROPERTY()
	FString DefaultGameSlot = "GameSlot_0";

	UPROPERTY()
	FString DefaultPlayerSlot = "PlayerSlot_0";	

	UPROPERTY()
	bool bNewGame = false;

	UPROPERTY()
	bool HasGameData = false;

	UPROPERTY(EditAnywhere)
	TMap<FName, bool> CanEnterStage; // Stage이름, 입장가능여부
};
