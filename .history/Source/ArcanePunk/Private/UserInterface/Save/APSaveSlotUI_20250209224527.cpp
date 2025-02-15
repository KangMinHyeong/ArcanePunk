
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
#include "UserInterface/Common/Check/APCheckUI.h"
#include "GameInstance/APGameInstance.h"

void UAPSaveSlotUI::NativeConstruct()
{
    Super::NativeConstruct();

    SetIsFocusable(true);
    SetKeyboardFocus();

    UAPSoundSubsystem::PlayUIOpenSound(UAPGameInstance::GetSoundGI(GetWorld()));
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

FReply UAPSaveSlotUI::NativeOnKeyDown(const FGeometry &InGeometry, const FKeyEvent &InKeyEvent)
{
    FReply Reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

    if(InKeyEvent.GetKey() == EKeys::Escape)
    {
        RemoveFromParent();
        return Reply.Handled();
    }
    
	return Reply.Handled();
}

void UAPSaveSlotUI::OnValidating(ECheckType UpdateCheckType)
{
    if(!CurrentSaveSlot.IsValid()) return;

    switch (UpdateCheckType)
    {
    case ECheckType::Delete:
        CurrentSaveSlot->Delete();
        break;
    
    case ECheckType::Save:
        CurrentSaveSlot->Save();
        break;

    case ECheckType::Load:
        CurrentSaveSlot->Load();
        break;
    }    
}

void UAPSaveSlotUI::BindButton()
{
    Button_Back->OnClicked.AddDynamic(this, &UAPSaveSlotUI::OnClickButton_Back);
    Button_Delete->OnClicked.AddDynamic(this, &UAPSaveSlotUI::OnClickButton_Delete);
    Button_Select->OnClicked.AddDynamic(this, &UAPSaveSlotUI::OnClickButton_Select);

    Button_Back->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    Button_Delete->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    Button_Select->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    
    auto TitlePC = Cast<AAPTitlePlayerController>(GetOwningPlayer());
    if(TitlePC) {IsTitle = true;}
    else {IsTitle = false;}
}

void UAPSaveSlotUI::BindSlot()
{
    if(IsTitle)
    {
        UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Select, EStringRowName::Load);
    }
    else
    {
        UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Select, EStringRowName::Save);
    }
    
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Delete, EStringRowName::Delete);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_SaveSlot, EStringRowName::SaveSlotName);

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
    UAPSoundSubsystem::PlayUICloseSound(UAPGameInstance::GetSoundGI(GetWorld()));
    
    RemoveFromParent();

    auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); 
    if(PC) PC->CloseSaveSlot();

    auto TitlePC = Cast<AAPTitlePlayerController>(GetOwningPlayer());
    if(TitlePC) TitlePC->CreateTitleUI();    
}

void UAPSaveSlotUI::OnClickButton_Delete()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    OpenCheckUI(ECheckType::Delete);
}

void UAPSaveSlotUI::OnClickButton_Select()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(!CurrentSaveSlot.IsValid()) return;
    if(IsTitle) 
    {
        if(!CurrentSaveSlot->HasSavingData()) return;
        OpenCheckUI(ECheckType::Load);
    }
    else {OpenCheckUI(ECheckType::Save);}
}

void UAPSaveSlotUI::OpenCheckUI(ECheckType UpdateCheckType)
{
    auto DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return; 

    auto CheckUI = CreateWidget<UAPCheckUI>(GetWorld(), DataTableGI->GetCheckUIClass());
	if(CheckUI)
    {
        CheckUI->AddToViewport(1); 
        CheckUI->SetCheckType(UpdateCheckType, this);
    }
}

