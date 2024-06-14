// Fill out your copyright notice in the Description page of Project Settings.


#include "Save/APSaveGame.h"
#include "ArcanePunk/Public/Components/APInventoryComponent.h"

UAPSaveGame::UAPSaveGame()
{
}

void UAPSaveGame::InitSaveData()
{
    SavePlayerTotalData = {};

    SavePlayerSlot = "PlayerSlot_0";

    //GameData
    SaveInGameData = {};
}
