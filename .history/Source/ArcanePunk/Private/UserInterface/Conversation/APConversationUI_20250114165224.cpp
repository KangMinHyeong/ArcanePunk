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

FReply UAPConversationUI::NativeOnMouseButtonDoubleClick( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) 
{
    FReply Reply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
    
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

void UAPConversationUI::InitOrder(const int32 & NewGroupID, bool bTutorial, UAPTutorialUI* UI)
{
    bTutorialUI = bTutorial;
    TutorialUI = UI;   

    auto DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return;   
    
    GroupID = NewGroupID; 
    Dialogues = DataTableGI->GetDialogues(GroupID);
    CharacterUIData = DataTableGI->GetCharacterUIDataTable();

    Message_Sort = 0;
    Message_Num = Dialogues.Num();

    SetOrder();
    ConversationTextUI->SetTickEnable(!bTutorialUI);
}

void UAPConversationUI::SetOrder()
{
    
        // if(bTutorialUI)
        // {
        //     if(TutorialUI.IsValid())
        //     {
        //          TutorialUI->OnSwitchingGuide();
        //          TutorialUI = nullptr;
        //     }
        // }
        // else
        // {
        //     auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer());
        //     if(PC) PC->CloseConversationUI(InitName);
        // }
    
    if(Message_Sort >= Message_Num)
    {
        if(bTutorialUI)
        {
            if(TutorialUI.IsValid())
            {
                 TutorialUI->OnSwitchingGuide();
                 TutorialUI = nullptr;
            }
        }
        else
        {
            auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer());
            if(PC) PC->CloseConversationUI(GroupID);
        }
    }
    else
    {
        auto Data = CharacterUIData->FindRow<FCharaterUIData>(FName(*FString::FromInt(Dialogues[Message_Sort].Character_ID)), TEXT("None")); 
        ConversationTextUI->SetConversation(Dialogues[Message_Sort], *Data); 
        Message_Sort++;
    }   
}

void UAPConversationUI::ConversationCompleteCheck()
{
    if(ConversationTextUI->bTextComplete)
    {
        // SetOrder();
    }
    else
    {
        ConversationTextUI->SkipConversation();
    }
}

void UAPConversationUI::StartTextFlow()
{
    ConversationTextUI->SetTickEnable(true);
}