
#include "UserInterface/Save/APSaveSlotUI.h"

#include "Save/APSaveGame.h"
#include "Components/Button.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "PlayerState/ArcanePunkPlayerState.h"
#include "GameState/APGameState.h"
#include "Kismet/GameplayStatics.h"
#include "UserInterface/Save/APSaveDataSlot.h"
#include "Components/ScrollBox.h"
#include "PlayerController/APTitlePlayerController.h"
#include "Components/TextBlock.h"

void UAPSaveSlotUI::NativeConstruct()
{
    Super::NativeConstruct();

    SetIsFocusable(true);
    SetKeyboardFocus();
}

FReply UAPSaveSlotUI::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    return Reply.Handled();
}

FReply UAPSaveSlotUI::NativeOnMouseButtonDoubleClick(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
    return Reply.Handled();
}

FReply UAPSaveSlotUI::NativeOnMouseWheel( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
    FReply Reply = Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
	return Reply.Handled();
}

void UAPSaveSlotUI::BindButton()
{
    Button_Back->OnClicked.AddDynamic(this, &UAPSaveSlotUI::OnClickButton_Back);
    Button_Delete->OnClicked.AddDynamic(this, &UAPSaveSlotUI::OnClickButton_Delete);
    Button_Select->OnClicked.AddDynamic(this, &UAPSaveSlotUI::OnClickButton_Select);

    auto TitlePC = Cast<AAPTitlePlayerController>(GetOwningPlayer());
    if(TitlePC) {IsTitle = true;}
    else {IsTitle = false;}
}

void UAPSaveSlotUI::BindSlot()
{
    if(IsTitle)
    {
        FString LoadText = TEXT("불러오기");
        TextBlock_Select->SetText(FText::FromString(LoadText));
    }
    
    OnSlot();
}

void UAPSaveSlotUI::ChangingCurrentSaveSlot(UUserWidget *ClickedSlot)
{
    if(CurrentSaveSlot.IsValid()) CurrentSaveSlot->TurnOffSlot();
    CurrentSaveSlot = Cast<UAPSaveDataSlot>(ClickedSlot);
}

void UAPSaveSlotUI::OnSlot()
{
    ScrollBox->ClearChildren();

    uint8 SlotNum = 1;
    for(auto SlotName : SlotNames)
    {
        UAPSaveGame* SaveGameData = Cast<UAPSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
        
        UAPSaveDataSlot* SaveDataSlot = nullptr;
        if(SaveGameData)
        {
            SaveDataSlot = Cast<UAPSaveDataSlot>(CreateWidget(this, SaveDataSlotClass)); if(!SaveDataSlot) return;
            SaveDataSlot->SetSlotData(SaveGameData);
        }
        else
        {
            SaveDataSlot = Cast<UAPSaveDataSlot>(CreateWidget(this, NoDataSlotClass)); if(!SaveDataSlot) return;
        }
        SaveDataSlot->SetSlotName(SlotName, this);
        SaveDataSlot->SetSlotNumber(SlotNum);
        ScrollBox->AddChild(SaveDataSlot);
        SlotNum++;
    }
}

void UAPSaveSlotUI::OnClickButton_Back()
{
    RemoveFromParent();

    auto TitlePC = Cast<AAPTitlePlayerController>(GetOwningPlayer());
    if(TitlePC) TitlePC->CreateTitleUI();    
}

void UAPSaveSlotUI::OnClickButton_Delete()
{
    if(!CurrentSaveSlot.IsValid()) return;
    CurrentSaveSlot->Delete();
}

void UAPSaveSlotUI::OnClickButton_Select()
{
    if(!CurrentSaveSlot.IsValid()) return;

    if(IsTitle) {CurrentSaveSlot->Load();}
    else {CurrentSaveSlot->Save();}
}
