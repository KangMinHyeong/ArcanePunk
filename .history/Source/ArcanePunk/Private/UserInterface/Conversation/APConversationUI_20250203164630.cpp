// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Conversation/APConversationUI.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "UserInterface/Conversation/APConversationText.h"
#include "GameInstance/APGameInstance.h"
#include "UserInterface/Tutorial/APTutorialUI.h"
#include "GameMode/APGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UAPConversationUI::NativeOnInitialized()
{
    Super::NativeOnInitialized();
}

void UAPConversationUI::NativeConstruct()
{
    Super::NativeConstruct();

    UAPSoundSubsystem::PlayUIOpenSound(UAPGameInstance::GetSoundGI(GetWorld()));
}

FReply UAPConversationUI::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent) // 클릭 아이템 인벤토리로 (오른쪽 마우스)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    
    if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && bCanSkip)
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

    if(InKeyEvent.GetKey() == EKeys::F && !bKeyPress && bCanSkip)
    {
        bKeyPress = true;
        ConversationTextUI->bSkipPress = true;
        return Reply.Handled();
    }
    
	return Reply.Handled();
}

FReply UAPConversationUI::NativeOnKeyUp(const FGeometry &InGeometry, const FKeyEvent &InKeyEvent)
{
    FReply Reply = Super::NativeOnKeyUp(InGeometry, InKeyEvent);

    if(InKeyEvent.GetKey() == EKeys::F && bCanSkip)
    {
        if(ConversationTextUI->SkipPressTime <= 0.2f)
        {
            ConversationTextUI->InitFullSkip();
            ConversationCompleteCheck();
        }
        else
        {
            if(ConversationTextUI->SkipPressTime <= 3.0f)
            {
                ConversationTextUI->InitFullSkip();
            }
        }

        bKeyPress = false;
        return Reply.Handled();
    }

    if(InKeyEvent.GetKey() == EKeys::SpaceBar && bCanSkip)
    {
        ConversationCompleteCheck();
        
        return Reply.Handled();
    }

    return Reply.Handled();
}

void UAPConversationUI::InitOrder(const int32 & NewGroupID, bool bTutorial)
{
    SetIsFocusable(true);
    SetKeyboardFocus();

    bTutorialUI = bTutorial;

    auto DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return;   
    
    GroupID = NewGroupID; 
    Dialogues = DataTableGI->GetDialogues(GroupID);
    CharacterUIData = DataTableGI->GetCharacterUIDataTable();

    Message_Sort = 0;
    Message_Num = Dialogues.Num();

    SetOrder();
    StartTextFlow(!bTutorialUI);

    if(!bTutorialUI) OnFadeInConversation();
}

void UAPConversationUI::SetOrder()
{    
    if(Message_Sort >= Message_Num)
    {
        bCanSkip = false;
        if(!bTutorialUI) {OnFadeOutConversation();}
        else {OnEndedConversation();}      
    }
    else
    {
        auto Data = CharacterUIData->FindRow<FCharaterUIData>(FName(*FString::FromInt(Dialogues[Message_Sort].Character_ID)), TEXT("None")); 
        ConversationTextUI->SetConversation(this, Dialogues[Message_Sort], *Data); 
        Message_Sort++;
    }   
}

void UAPConversationUI::ConversationCompleteCheck()
{
    if(ConversationTextUI->bTextComplete)
    {
        SetOrder();
    }
    else
    {
        ConversationTextUI->SkipConversation();
    }
}

void UAPConversationUI::StartTextFlow(bool NewBool)
{
    ConversationTextUI->SetTickEnable(NewBool);
    bCanSkip = NewBool;
}

void UAPConversationUI::StartFullSkip()
{
    Message_Sort = Message_Num;
    SetOrder();
}

void UAPConversationUI::OnEndedConversation()
{
    auto GM = Cast<AAPGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
    if(GM && GM->OnEndedConversation.IsBound()) GM->OnEndedConversation.Broadcast(GroupID);
}