// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Conversation/APConversationUI.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "UserInterface/Conversation/APConversationText.h"

void UAPConversationUI::NativeOnInitialized()
{
    Super::NativeOnInitialized();
}

void UAPConversationUI::NativeConstruct()
{
    Super::NativeConstruct();
    SetIsFocusable(true);
    SetKeyboardFocus();
}

FReply UAPConversationUI::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent) // 클릭 아이템 인벤토리로 (오른쪽 마우스)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    
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

void UAPConversationUI::InitOrder(FName Name, uint8 State)
{
    auto DataTable = ConversationDataTable->FindRow<FConversationDataTable>(Name, Name.ToString()); if(!DataTable){RemoveFromParent(); return;}
    TArray<FConversationData> ConversationOrder;
    if(DataTable->ConversationPartnerPrompt.Num() > State) ConversationOrder = DataTable->ConversationPartnerPrompt[State].ConversationDataArray;
    if(ConversationOrder.IsEmpty()) {RemoveFromParent(); return;}

    Order = ConversationOrder;
    SetOrder();
    
    // ConversationText->
    // ConversationOrder[0].
}

void UAPConversationUI::SetOrder()
{
    if(Order.Num() > TextOrder) {ConversationText->SetConversation(Order[TextOrder]);}
    else
    {
        auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer());
        if(PC) PC->CloseConversationUI();
    }
}

void UAPConversationUI::ConversationCompleteCheck()
{
    if(ConversationText->bTextComplete)
    {
        TextOrder++;
        SetOrder();
    }
    else
    {
        ConversationText->SkipConversation(Order[TextOrder]);
    }
    
}