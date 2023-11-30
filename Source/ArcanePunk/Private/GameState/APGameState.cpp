// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/APGameState.h"

#include "Kismet/GameplayStatics.h"
#include "Save/APSaveGame.h"

void AAPGameState::SaveGameData()
{
    UAPSaveGame* SaveGameData = NewObject<UAPSaveGame>();
    
    SaveGameData->SaveLevelName = GameData.LevelName;

	if (!UGameplayStatics::SaveGameToSlot(SaveGameData, SaveSlotName, 0))
	{
		UE_LOG(LogTemp, Display, TEXT("Save Fail"));
	}
}

void AAPGameState::InitGameData()
{
    UAPSaveGame* SaveGameData = Cast<UAPSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (!SaveGameData)
	{
		SaveGameData = GetMutableDefault<UAPSaveGame>();
	}

    GameData.LevelName = SaveGameData->SaveLevelName;
}

void AAPGameState::UpdateGameData(FGameData& GD)
{
    GameData = GD;

    SaveGameData();
}
