// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/ArcanePunkPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Save/APSaveGame.h"

AArcanePunkPlayerState::AArcanePunkPlayerState()
{
}

void AArcanePunkPlayerState::SavePlayerData()
{
    UAPSaveGame* SaveGameData = NewObject<UAPSaveGame>();
    
    SaveGameData->bSave = PlayerStatus.SaveOperation;
    SaveGameData->SaveHP = PlayerStatus.HP;
    SaveGameData->SaveMaxHP = PlayerStatus.MaxHP;
	SaveGameData->SaveMP = PlayerStatus.MP;
    SaveGameData->SaveMaxMP = PlayerStatus.MaxMP;
	SaveGameData->SaveATK = PlayerStatus.ATK;
	SaveGameData->SaveATKSpeed = PlayerStatus.ATKSpeed;
    SaveGameData->SaveDEF = PlayerStatus.DEF;
	SaveGameData->SaveMoveSpeed = PlayerStatus.MoveSpeed;
    SaveGameData->SaveSP = PlayerStatus.SP;
    SaveGameData->SavePlayerLocation = PlayerStatus.PlayerLocation;
    
	if (!UGameplayStatics::SaveGameToSlot(SaveGameData, SaveSlotName, 0))
	{
		UE_LOG(LogTemp, Display, TEXT("Save Fail"));
	}
}

void AArcanePunkPlayerState::InitPlayerData()
{
    UAPSaveGame* SaveGameData = Cast<UAPSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (!SaveGameData)
	{
		SaveGameData = GetMutableDefault<UAPSaveGame>();
	}

    PlayerStatus.SaveOperation = SaveGameData->bSave;
    PlayerStatus.HP = SaveGameData->SaveHP;
    PlayerStatus.MaxHP = SaveGameData->SaveMaxHP;
	PlayerStatus.MP = SaveGameData->SaveMP;
    PlayerStatus.MaxMP = SaveGameData->SaveMaxMP;
	PlayerStatus.ATK = SaveGameData->SaveATK;
	PlayerStatus.ATKSpeed = SaveGameData->SaveATKSpeed;
    PlayerStatus.DEF = SaveGameData->SaveDEF;
	PlayerStatus.MoveSpeed = SaveGameData->SaveMoveSpeed;
    PlayerStatus.SP = SaveGameData->SaveSP;
    PlayerStatus.PlayerLocation = SaveGameData->SavePlayerLocation;
}

void AArcanePunkPlayerState::UpdatePlayerData(FPlayerData &PS)
{
    PlayerStatus.SaveOperation = true;
    PlayerStatus.HP = PS.HP;
    PlayerStatus.MaxHP = PS.MaxHP;
    PlayerStatus.MP = PS.MP;
    PlayerStatus.MaxMP = PS.MaxMP;
    PlayerStatus.ATK = PS.ATK;
    PlayerStatus.ATKSpeed = PS.ATKSpeed;
    PlayerStatus.DEF = PS.DEF;
    PlayerStatus.MoveSpeed = PS.MoveSpeed;
    PlayerStatus.SP = PS.SP;
	PlayerStatus.PlayerLocation = PS.PlayerLocation;

    SavePlayerData();
}
