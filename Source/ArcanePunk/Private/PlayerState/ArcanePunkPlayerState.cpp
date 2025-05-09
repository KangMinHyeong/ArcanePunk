
#include "PlayerState/ArcanePunkPlayerState.h"

#include "Kismet/GameplayStatics.h"
#include "Save/APSaveGame.h"
#include "ArcanePunk/Public/Components/Character/APInventoryComponent.h"
#include "GameInstance/APGameInstance.h"

AArcanePunkPlayerState::AArcanePunkPlayerState()
{
}

void AArcanePunkPlayerState::SavePlayerData()
{
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;

    GI->GetAPSaveGameData()->SavePlayerTotalData = PlayerTotalStatus;

	if (!UGameplayStatics::SaveGameToSlot(GI->GetAPSaveGameData(), PlayerTotalStatus.SaveSlotName, 0))
	{
		UE_LOG(LogTemp, Display, TEXT("Save Fail"));
	}
}

void AArcanePunkPlayerState::InitPlayerData()
{
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;

    UAPSaveGame* SaveGameData = Cast<UAPSaveGame>(UGameplayStatics::LoadGameFromSlot(GI->GetDefaultSlotName(), 0));
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
    PlayerTotalStatus.SaveOperation = true;

    SavePlayerData();
}

void AArcanePunkPlayerState::DeathPenalty()
{
    PlayerTotalStatus.PlayerDeathCount++;

    SavePlayerData();
}
