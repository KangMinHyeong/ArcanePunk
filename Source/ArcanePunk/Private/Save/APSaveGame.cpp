// Fill out your copyright notice in the Description page of Project Settings.


#include "Save/APSaveGame.h"

void UAPSaveGame::InitSaveData()
{
    //PlayerData
    bSave = false;
    SaveHP = 100.0f;
	SaveMP = 100;
	SaveATK = 25.0f;
	SaveATKSpeed = 1;
    SaveDEF = 0.0f;
	SaveMoveSpeed = 500.0f;
    SaveSP = 500.0f;
    SavePlayerLocation = FVector(0,0,0);

    //GameData
    SaveLevelName = TEXT("Test1");
}
