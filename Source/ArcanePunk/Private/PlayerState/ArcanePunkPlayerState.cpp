
#include "PlayerState/ArcanePunkPlayerState.h"

#include "Kismet/GameplayStatics.h"
#include "Save/APSaveGame.h"
#include "ArcanePunk/Public/Components/APInventoryComponent.h"
#include "GameInstance/APGameInstance.h"

AArcanePunkPlayerState::AArcanePunkPlayerState()
{
}

void AArcanePunkPlayerState::SavePlayerData()
{
    UAPSaveGame* SaveGameData = NewObject<UAPSaveGame>();
    
    //SaveGameData->SaveRerollDice = PlayerTotalStatus.PlayerGoodsData.RerollDice;

    SaveGameData->SavePlayerTotalData = PlayerTotalStatus;

	if (!UGameplayStatics::SaveGameToSlot(SaveGameData, PlayerTotalStatus.SaveSlotName, 0))
	{
		UE_LOG(LogTemp, Display, TEXT("Save Fail"));
	}
}

void AArcanePunkPlayerState::InitPlayerData()
{
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;

    UAPSaveGame* SaveGameData = Cast<UAPSaveGame>(UGameplayStatics::LoadGameFromSlot(GI->GetDefaultPlayerSlot(), 0));
	if (!SaveGameData)
	{
		SaveGameData = GetMutableDefault<UAPSaveGame>();
	}  

    PlayerTotalStatus = SaveGameData->SavePlayerTotalData;

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
    
    //Goods
    // 

    SavePlayerData();
}
