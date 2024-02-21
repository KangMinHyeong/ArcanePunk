
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

void UAPSaveSlotUI::NativeConstruct()
{
    Super::NativeConstruct();

    // BindButton();
}

FReply UAPSaveSlotUI::NativeOnMouseWheel( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
    FReply Reply = Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
	return Reply.Handled();
}

void UAPSaveSlotUI::BindButton()
{
    Button_Back->OnClicked.AddDynamic(this, &UAPSaveSlotUI::OnClickButton_Back);

    auto TitlePC = Cast<AAPTitlePlayerController>(GetOwningPlayer());
    if(TitlePC) {IsTitle = true;}
    else {IsTitle = false;}

}

void UAPSaveSlotUI::BindSlot()
{
    auto TitlePC = Cast<AAPTitlePlayerController>(GetOwningPlayer());
    if(TitlePC) {IsTitle = true;}
    else {IsTitle = false;}

    ScrollBox->ClearChildren();

    OnSlot1();
    OnSlot2();
    OnSlot3();
    OnSlot4();
    OnSlot5();
}

void UAPSaveSlotUI::OnSlot1()
{
    FString SlotName = "PlayerSlot_1";
    UAPSaveGame* SaveGameData = Cast<UAPSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    if(SaveGameData)
    {
        auto SaveDataSlot = Cast<UAPSaveDataSlot>(CreateWidget(this, SaveDataSlotClass)); if(!SaveDataSlot) return;
        SaveDataSlot->SetSlotName(SlotName);
        SaveDataSlot->SetSlotNumber(1);
        SaveDataSlot->SetLoadMode(IsTitle);
        ScrollBox->AddChild(SaveDataSlot);
        // To Do : 저장된 슬롯 띄우기
        // 슬롯에 PlayerSlot_1 값 넘겨주기
    }
    else
    {
        auto NoDataSlot = Cast<UAPSaveDataSlot>(CreateWidget(this, NoDataSlotClass)); if(!NoDataSlot) return;
        NoDataSlot->SetLoadMode(IsTitle);
        NoDataSlot->SetSlotNumber(1);
        ScrollBox->AddChild(NoDataSlot);
    }
}

void UAPSaveSlotUI::OnSlot2()
{
    FString SlotName = "PlayerSlot_2";
    UAPSaveGame* SaveGameData = Cast<UAPSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    if(SaveGameData)
    {
        auto SaveDataSlot = Cast<UAPSaveDataSlot>(CreateWidget(this, SaveDataSlotClass)); if(!SaveDataSlot) return;
        SaveDataSlot->SetSlotName(SlotName);
        SaveDataSlot->SetSlotNumber(2);
        SaveDataSlot->SetLoadMode(IsTitle);
        ScrollBox->AddChild(SaveDataSlot);
        // To Do : 저장된 슬롯 띄우기
        // 슬롯에 PlayerSlot_1 값 넘겨주기
    }
    else
    {
        auto NoDataSlot = Cast<UAPSaveDataSlot>(CreateWidget(this, NoDataSlotClass)); if(!NoDataSlot) return;
        NoDataSlot->SetLoadMode(IsTitle);
        NoDataSlot->SetSlotNumber(2);
        ScrollBox->AddChild(NoDataSlot);
    }
}

void UAPSaveSlotUI::OnSlot3()
{
    FString SlotName = "PlayerSlot_3";
    UAPSaveGame* SaveGameData = Cast<UAPSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    if(SaveGameData)
    {
        auto SaveDataSlot = Cast<UAPSaveDataSlot>(CreateWidget(this, SaveDataSlotClass)); if(!SaveDataSlot) return;
        SaveDataSlot->SetSlotName(SlotName);
        SaveDataSlot->SetSlotNumber(3);
        SaveDataSlot->SetLoadMode(IsTitle);
        ScrollBox->AddChild(SaveDataSlot);
        // To Do : 저장된 슬롯 띄우기
        // 슬롯에 PlayerSlot_1 값 넘겨주기
    }
    else
    {
        auto NoDataSlot = Cast<UAPSaveDataSlot>(CreateWidget(this, NoDataSlotClass)); if(!NoDataSlot) return;
        NoDataSlot->SetLoadMode(IsTitle);
        NoDataSlot->SetSlotNumber(3);
        ScrollBox->AddChild(NoDataSlot);
    }
}

void UAPSaveSlotUI::OnSlot4()
{
    FString SlotName = "PlayerSlot_4";
    UAPSaveGame* SaveGameData = Cast<UAPSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    if(SaveGameData)
    {
        auto SaveDataSlot = Cast<UAPSaveDataSlot>(CreateWidget(this, SaveDataSlotClass)); if(!SaveDataSlot) return;
        SaveDataSlot->SetSlotName(SlotName);
        SaveDataSlot->SetSlotNumber(4);
        SaveDataSlot->SetLoadMode(IsTitle);
        ScrollBox->AddChild(SaveDataSlot);
        // To Do : 저장된 슬롯 띄우기
        // 슬롯에 PlayerSlot_1 값 넘겨주기
    }
    else
    {
        auto NoDataSlot = Cast<UAPSaveDataSlot>(CreateWidget(this, NoDataSlotClass)); if(!NoDataSlot) return;
        NoDataSlot->SetLoadMode(IsTitle);
        NoDataSlot->SetSlotNumber(4);
        ScrollBox->AddChild(NoDataSlot);
    }
}

void UAPSaveSlotUI::OnSlot5()
{
    FString SlotName = "PlayerSlot_5";
    UAPSaveGame* SaveGameData = Cast<UAPSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    if(SaveGameData)
    {
        auto SaveDataSlot = Cast<UAPSaveDataSlot>(CreateWidget(this, SaveDataSlotClass)); if(!SaveDataSlot) return;
        SaveDataSlot->SetSlotName(SlotName);
        SaveDataSlot->SetSlotNumber(5);
        SaveDataSlot->SetLoadMode(IsTitle);
        ScrollBox->AddChild(SaveDataSlot);
        // To Do : 저장된 슬롯 띄우기
        // 슬롯에 PlayerSlot_1 값 넘겨주기
    }
    else
    {
        auto NoDataSlot = Cast<UAPSaveDataSlot>(CreateWidget(this, NoDataSlotClass)); if(!NoDataSlot) return;
        NoDataSlot->SetLoadMode(IsTitle);
        NoDataSlot->SetSlotNumber(5);
        ScrollBox->AddChild(NoDataSlot);
    }
}

void UAPSaveSlotUI::OnClickButton_Back()
{
    RemoveFromParent();

    auto TitlePC = Cast<AAPTitlePlayerController>(GetOwningPlayer());
    if(TitlePC) TitlePC->CreateTitleUI();    
}
