// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Conversation/APConversationUI.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "UserInterface/Conversation/APConversationText.h"
#include "GameInstance/APGameInstance.h"
#include "UserInterface/Tutorial/APTutorialUI.h"

void UAPConversationUI::NativeOnInitialized()
{
    Super::NativeOnInitialized();
}

void UAPConversationUI::NativeConstruct()
{
    Super::NativeConstruct();
    SetIsFocusable(true);
    SetKeyboardFocus();

    UAPSoundSubsystem::PlayUIOpenSound(UAPGameInstance::GetSoundGI(GetWorld()));
}

FReply UAPConversationUI::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent) // 클릭 아이템 인벤토리로 (오른쪽 마우스)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    
    if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
        ConversationCompleteCheck();

        return Reply.Handled();
    }

    return Reply.Handled();
}

FReply UAPConversationUI::NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) // 전부 인벤토리로 (F키)
{
    FReply Reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

    if(InKeyEvent.GetKey() == EKeys::F || InKeyEvent.GetKey() == EKeys::SpaceBar)
    {
        ConversationCompleteCheck();
        
        return Reply.Handled();
    }
    
	return Reply.Handled();
}

void UAPConversationUI::InitOrder(const FName & Name, bool bTutorial, UAPTutorialUI* UI)
{
    bTutorialUI = bTutorial;
    TutorialUI = UI;

    InitName = Name; RowName = Name; 
    SetOrder();
    ConversationText->SetTickEnable(!bTutorialUI);
}

void UAPConversationUI::SetOrder()
{
    if(RowName == "None")
    {
        if(bTutorialUI)
        {
            if(TutorialUI.IsValid()) TutorialUI->OnSwitchingGuide();
        }
        else
        {
            auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer());
            if(PC) PC->CloseConversationUI(InitName);
        }
    }
    else
    {
        auto DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return;   
        auto DataTable = DataTableGI->GetSequenceStringDataTable()->FindRow<FSequenceStringDataTable>(RowName, RowName.ToString()); if(!DataTable) return;
        RowName = DataTable->NextRowName;
        ConversationText->SetConversation(DataTable);
    }
}

void UAPConversationUI::ConversationCompleteCheck()
{
    if(ConversationText->bTextComplete)
    {
        SetOrder();
    }
    else
    {
        ConversationText->SkipConversation();
    }
}

void UAPConversationUI::StartTextFlow()
{
    ConversationText->SetTickEnable(true);
}