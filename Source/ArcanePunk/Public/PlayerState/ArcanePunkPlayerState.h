// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ArcanePunkPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API AArcanePunkPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AArcanePunkPlayerState();
	

public:
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 MyCharacterIndex = 0;

	UPROPERTY(Transient, BlueprintReadWrite)
	FName CharacterName = TEXT("Player");

	UPROPERTY(Transient, BlueprintReadWrite)
	uint8 CharacterGroup = 0;//0:OC / 1:NPC

	UPROPERTY(Transient, BlueprintReadWrite)
	uint8 Gender = 0;//0: Male / 1:Female;

	UPROPERTY(Transient, BlueprintReadWrite)
	uint8 CharacterRace = 0; //0:Human

	UPROPERTY(Transient, BlueprintReadWrite)
	uint8 CharacterType = 0; //0:TypeA 

	UPROPERTY(Transient, BlueprintReadWrite)
	float HP = 100.0f;

	UPROPERTY(Transient, BlueprintReadWrite)
	int32 MP = 100;

	UPROPERTY(Transient, BlueprintReadWrite)
	float ATK = 25.0f;

	UPROPERTY(Transient, BlueprintReadWrite)
	int32 ATKSpeed = 1;

	UPROPERTY(Transient, BlueprintReadWrite)
	float DEF = 0.0f;

	UPROPERTY(Transient, BlueprintReadWrite)
	float MoveSpeed = 500.0f;

	UPROPERTY(Transient, BlueprintReadWrite)
	int32 SP = 500.0f;
};
