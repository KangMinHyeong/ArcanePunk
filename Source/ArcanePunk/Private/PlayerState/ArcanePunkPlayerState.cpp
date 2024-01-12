
#include "PlayerState/ArcanePunkPlayerState.h"

#include "Kismet/GameplayStatics.h"
#include "Save/APSaveGame.h"
#include "Components/Common/APCrowdControlComponent.h"

AArcanePunkPlayerState::AArcanePunkPlayerState()
{
}

void AArcanePunkPlayerState::SavePlayerData()
{
    UAPSaveGame* SaveGameData = NewObject<UAPSaveGame>();
    
    SaveGameData->bSave = PlayerTotalStatus.PlayerDynamicData.SaveOperation;
    SaveGameData->SaveHP = PlayerTotalStatus.PlayerDynamicData.HP;
    SaveGameData->SaveMaxHP = PlayerTotalStatus.PlayerDynamicData.MaxHP;
	SaveGameData->SaveMP = PlayerTotalStatus.PlayerDynamicData.MP;
    SaveGameData->SaveMaxMP = PlayerTotalStatus.PlayerDynamicData.MaxMP;
	SaveGameData->SaveATK = PlayerTotalStatus.PlayerDynamicData.ATK;
	SaveGameData->SaveATKSpeed = PlayerTotalStatus.PlayerDynamicData.ATKSpeed;
    SaveGameData->SaveDEF = PlayerTotalStatus.PlayerDynamicData.DEF;
	SaveGameData->SaveMoveSpeed = PlayerTotalStatus.PlayerDynamicData.MoveSpeed;
    SaveGameData->SaveSP = PlayerTotalStatus.PlayerDynamicData.SP;
    SaveGameData->SavePlayerLocation = PlayerTotalStatus.PlayerDynamicData.PlayerLocation;
    SaveGameData->SaveHasQSkillType = PlayerTotalStatus.PlayerDynamicData.HasQSkillType;
    SaveGameData->SaveHasESkillType = PlayerTotalStatus.PlayerDynamicData.HasESkillType;
    SaveGameData->SaveHasRSkillType = PlayerTotalStatus.PlayerDynamicData.HasRSkillType;

    //SaveGameData->SaveRerollDice = PlayerTotalStatus.PlayerGoodsData.RerollDice;

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

    PlayerTotalStatus.PlayerDynamicData.SaveOperation = SaveGameData->bSave;
    PlayerTotalStatus.PlayerDynamicData.HP = SaveGameData->SaveHP;
    PlayerTotalStatus.PlayerDynamicData.MaxHP = SaveGameData->SaveMaxHP;
	PlayerTotalStatus.PlayerDynamicData.MP = SaveGameData->SaveMP;
    PlayerTotalStatus.PlayerDynamicData.MaxMP = SaveGameData->SaveMaxMP;
	PlayerTotalStatus.PlayerDynamicData.ATK = SaveGameData->SaveATK;
	PlayerTotalStatus.PlayerDynamicData.ATKSpeed = SaveGameData->SaveATKSpeed;
    PlayerTotalStatus.PlayerDynamicData.DEF = SaveGameData->SaveDEF;
	PlayerTotalStatus.PlayerDynamicData.MoveSpeed = SaveGameData->SaveMoveSpeed;
    PlayerTotalStatus.PlayerDynamicData.SP = SaveGameData->SaveSP;
    PlayerTotalStatus.PlayerDynamicData.PlayerLocation = SaveGameData->SavePlayerLocation;
    PlayerTotalStatus.PlayerDynamicData.HasQSkillType = SaveGameData->SaveHasQSkillType;
    PlayerTotalStatus.PlayerDynamicData.HasESkillType = SaveGameData->SaveHasESkillType;
    PlayerTotalStatus.PlayerDynamicData.HasRSkillType = SaveGameData->SaveHasRSkillType;

    // Goods
    // PlayerTotalStatus.PlayerGoodsData.RerollDice = SaveGameData->SaveRerollDice;
}

void AArcanePunkPlayerState::UpdatePlayerData(FPlayerTotalData &PS)
{
    PlayerTotalStatus = PS;
    PlayerTotalStatus.PlayerDynamicData.SaveOperation = true;

    SavePlayerData();
}

void AArcanePunkPlayerState::DeathPenalty()
{
    // PlayerDynamicData
    PlayerTotalStatus.PlayerDynamicData.HasQSkillType = false;
    PlayerTotalStatus.PlayerDynamicData.HasESkillType = false;
    PlayerTotalStatus.PlayerDynamicData.HasRSkillType = false;

    //Goods
    // 

    SavePlayerData();
}
