
#include "UserInterface/Save/APSaveDataSlot.h"

#include "Components/TextBlock.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameState/APGameState.h"
#include "GameInstance/APGameInstance.h"
#include "Save/APSaveGame.h"
#include "PlayerController/ArcanePunkPlayerController.h"

void UAPSaveDataSlot::NativeConstruct()
{
    Super::NativeConstruct();

    TEXT_SlotName->SetText(FText::FromString(SlotName));
}

FReply UAPSaveDataSlot::NativeOnMouseButtonDoubleClick( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
    FReply Reply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

    if(LoadMode) // Title
    {
        if(SlotName == "No Data") // Load 데이터가 없으므로 리턴
        {
            return Reply.Handled();
        }
        else // Load 데이터가 있으므로 이 데이터로 시작
        {
            Load();
        }
    }
    else // InGame
    {
        if(SlotName == "No Data") // 유효 데이터가 없으므로 바로 저장
        {
            Save();
        }
        else // 유효 데이터가 있으므로 덮어씌울 지 되묻고 저장
        {
            Save();
        }
    }
    

	return Reply.Handled();
}

void UAPSaveDataSlot::SetSlotName(FString NewName)
{
    SlotName = NewName;
    TEXT_SlotName->SetText(FText::FromString(SlotName));
}

void UAPSaveDataSlot::SetSlotNumber(uint8 NewSlotNumber)
{
    SlotNumber = NewSlotNumber;
    TEXT_SlotNumber->SetText(FText::FromString(FString::FromInt(NewSlotNumber)));
}

void UAPSaveDataSlot::Save()
{
    auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter) return;

    switch (SlotNumber)
    {
        case 1:
        OwnerCharacter->SaveStatus(PlayerSlotName_1, GameSlotName_1);
        break;

        case 2:
        OwnerCharacter->SaveStatus(PlayerSlotName_2, GameSlotName_2);
        break;

        case 3:
        OwnerCharacter->SaveStatus(PlayerSlotName_3, GameSlotName_3);
        break;

        case 4:
        OwnerCharacter->SaveStatus(PlayerSlotName_4, GameSlotName_4);
        break;

        case 5:
        OwnerCharacter->SaveStatus(PlayerSlotName_5, GameSlotName_5);
        break;
    }

    auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); 
    if(PC) PC->CloseSaveSlot();
}

void UAPSaveDataSlot::Load()
{
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;

    switch (SlotNumber)
    {
        case 1:
        GI->DefaultGameSlot = GameSlotName_1;
        GI->DefaultPlayerSlot = PlayerSlotName_1;
        break;

        case 2:
        GI->DefaultGameSlot = GameSlotName_2;
        GI->DefaultPlayerSlot = PlayerSlotName_2;
        break;

        case 3:
        GI->DefaultGameSlot = GameSlotName_3;
        GI->DefaultPlayerSlot = PlayerSlotName_3;
        break;

        case 4:
        GI->DefaultGameSlot = GameSlotName_4;
        GI->DefaultPlayerSlot = PlayerSlotName_4;
        break;

        case 5:
        GI->DefaultGameSlot = GameSlotName_5;
        GI->DefaultPlayerSlot = PlayerSlotName_5;
        break;
    }

    UAPSaveGame* SaveGameData = Cast<UAPSaveGame>(UGameplayStatics::LoadGameFromSlot(GI->DefaultGameSlot, 0));
	if (!SaveGameData)
	{
		SaveGameData = GetMutableDefault<UAPSaveGame>();
	}
    UGameplayStatics::OpenLevel(this, SaveGameData->SaveLevelName);
    

    // auto GS = Cast<AAPGameState>(UGameplayStatics::GetGameState(this));
	// if(GS != nullptr)
	// {
    //     UGameplayStatics::OpenLevel(this, GS->GameData.LevelName);
    // }	    
}