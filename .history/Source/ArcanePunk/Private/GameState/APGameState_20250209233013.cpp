// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/APGameState.h"

#include "Kismet/GameplayStatics.h"
#include "Save/APSaveGame.h"
#include "GameInstance/APGameInstance.h"

void AAPGameState::SaveGameData()
{
	auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;
        
    GI->GetAPSaveGameData()->SaveInGameData = GameData;

	if (!UGameplayStatics::SaveGameToSlot(GI->GetAPSaveGameData(), GameData.SaveSlotName, 0))
	{
		UE_LOG(LogTemp, Display, TEXT("Save Fail"));
	}
}

void AAPGameState::InitGameData()
{
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;

    UAPSaveGame* SaveGameData = Cast<UAPSaveGame>(UGameplayStatics::LoadGameFromSlot(GI->GetDefaultSlotName(), 0));
	if (!SaveGameData)
	{
		SaveGameData = GetMutableDefault<UAPSaveGame>();
		UE_LOG(LogTemp, Display, TEXT("Your not load"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Your success load"));
	}

    GameData = SaveGameData->SaveInGameData;
}

void AAPGameState::UpdateGameData(FGameData& GD)
{
    GameData = GD;

    SaveGameData();
}
