#include "UserInterface/HUD/Status/APStatusText.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/TextBlock.h"
#include "GameFramework/Controller.h"
#include "PlayerController/ArcanePunkPlayerController.h"

void UAPStatusText::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!PC) return;

    PC->OnUpdateStatusText.AddUObject(this, &UAPStatusText::UpdatePlayerStatus);

}

void UAPStatusText::NativeConstruct()
{
    Super::NativeConstruct();
    UpdatePlayerStatus();
}

void UAPStatusText::UpdatePlayerStatus()
{
    auto Character = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn());
    if(!Character) return;
    CharacterIndex->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().StatusData.CharacterIndex)));
    CharacterName->SetText(FText::FromName(Character->GetPlayerStatus().StatusData.CharacterName));
    CharacterGroup->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().StatusData.CharacterGroup)));
    // Gender->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().PlayerStaticData.Gender)));
    // CharacterRace->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().PlayerStaticData.CharacterRace)));
    // CharacterType->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().PlayerStaticData.CharacterType)));
    HP->SetText(FText::FromString(FString::FromInt(round(Character->GetPlayerStatus().StatusData.HP))));
    MaxHP->SetText(FText::FromString(FString::FromInt(round(Character->GetPlayerStatus().StatusData.MaxHP))));
    MP->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().StatusData.MP)));
    MaxMP->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().StatusData.MaxMP)));
    ATK->SetText(FText::FromString(FString::FromInt(round(Character->GetCurrentATK()))));
    ATKSpeed->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), Character->GetPlayerStatus().StatusData.ATKSpeed)));
    DEF->SetText(FText::FromString(FString::FromInt(round(Character->GetPlayerStatus().StatusData.DEF))));
    MoveSpeed->SetText(FText::FromString(FString::FromInt(round(Character->GetPlayerStatus().StatusData.MoveSpeed))));
    SP->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().StatusData.SP)));
}