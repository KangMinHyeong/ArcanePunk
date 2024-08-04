
#include "UserInterface/Save/APSaveDataSlot.h"

#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameState/APGameState.h"
#include "GameInstance/APGameInstance.h"
#include "Save/APSaveGame.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "UserInterface/Save/APSaveSlotUI.h"

void UAPSaveDataSlot::NativeConstruct()
{
    Super::NativeConstruct();

}

FReply UAPSaveDataSlot::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    if(ParentWidget.IsValid()) ParentWidget->ChangingCurrentSaveSlot(this);
    TurnOnSlot();

    return Reply.Handled();
}

FReply UAPSaveDataSlot::NativeOnMouseButtonDoubleClick( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
    FReply Reply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	return Reply.Handled();
}

void UAPSaveDataSlot::SetSlotData(UAPSaveGame * SavedData)
{
    bSavingData = true;
    
    int32 RealTime = round(SavedData->SaveInGameData.SaveRealTime);
    TEXT_PlayTime_Hours->SetText(FText::FromString(FString::FromInt(RealTime / 3600))); RealTime = RealTime % 3600;
    TEXT_PlayTime_Minutes->SetText(FText::FromString(FString::FromInt(RealTime / 60))); RealTime = RealTime % 60;
    TEXT_PlayTime_Seconds->SetText(FText::FromString(FString::FromInt(RealTime)));

    TEXT_Year->SetText(FText::FromString(FString::FromInt(SavedData->SaveInGameData.SaveDateTime.GetYear()))); 
    TEXT_Month->SetText(FText::FromString(FString::FromInt(SavedData->SaveInGameData.SaveDateTime.GetMonth()))); 
    TEXT_Day->SetText(FText::FromString(FString::FromInt(SavedData->SaveInGameData.SaveDateTime.GetDay())));
    TEXT_Hours->SetText(FText::FromString(FString::FromInt(SavedData->SaveInGameData.SaveDateTime.GetHour()))); 
    TEXT_Minutes->SetText(FText::FromString(FString::FromInt(SavedData->SaveInGameData.SaveDateTime.GetMinute()))); 
    
    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;  
    auto Name = SavedData->SaveInGameData.LevelName;

    APGI->SetTextBlock(TEXT_SlotName, EStringRowName::NoData);
    APGI->SetTextBlock_Name(TEXT_SlotName, FName(*Name));
    
    APGI->SetTextBlock(Text_Slot, EStringRowName::Slot);
    APGI->SetTextBlock(Text_PlayTime, EStringRowName::PlayTime);
    APGI->SetTextBlock(Text_Hour, EStringRowName::Hour);
    APGI->SetTextBlock(Text_Minute, EStringRowName::Minute);
    APGI->SetTextBlock(Text_Second, EStringRowName::Second);
}

void UAPSaveDataSlot::SetSlotName(FString PlayerSlot, UUserWidget* Parent)
{
    PlayerSlotName = PlayerSlot;
    ParentWidget = Cast<UAPSaveSlotUI>(Parent);
}

void UAPSaveDataSlot::SetSlotNumber(uint8 NewSlotNumber)
{
    TEXT_SlotNumber->SetText(FText::FromString(FString::FromInt(NewSlotNumber)));
}

void UAPSaveDataSlot::TurnOnSlot()
{
    Border_Background->Background.SetResourceObject(TabImage_ON);
}

void UAPSaveDataSlot::TurnOffSlot()
{
    Border_Background->Background.SetResourceObject(TabImage_OFF);
}

void UAPSaveDataSlot::Save()
{
    auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter) return;

    OwnerCharacter->SaveStatus(PlayerSlotName);
    
    auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); 
    if(PC) PC->CloseSaveSlot();
}

void UAPSaveDataSlot::Load()
{
    if(!bSavingData) return;
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;

    GI->SetDefaultSlotName(PlayerSlotName);
    
    UAPSaveGame* SaveGameData = Cast<UAPSaveGame>(UGameplayStatics::LoadGameFromSlot(PlayerSlotName, 0));
	if (!SaveGameData)
	{
		SaveGameData = GetMutableDefault<UAPSaveGame>();
	}
    
    UGameplayStatics::OpenLevel(this, FName(*SaveGameData->SaveInGameData.LevelName));
}

void UAPSaveDataSlot::Delete()
{
    UGameplayStatics::DeleteGameInSlot(PlayerSlotName, 0);

    auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); 
    if(PC) PC->CloseSaveSlot();
}