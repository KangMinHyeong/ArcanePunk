// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "APGameInstance.generated.h"

// 후에 쓸 수도?
USTRUCT(BlueprintType)
struct FGameSoundVolume
{
	GENERATED_USTRUCT_BODY()

	FGameSoundVolume() {}

	UPROPERTY()
	float MasterVolume = 1.0f;
	UPROPERTY()
	float BGMVolume = 1.0f;
	UPROPERTY()
	float EffectVolume = 1.0f;
};

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

	UPROPERTY()
	FGameSoundVolume GameSoundVolume;
};
