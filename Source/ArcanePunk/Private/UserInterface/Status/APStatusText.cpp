#include "UserInterface/Status/APStatusText.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/TextBlock.h"
#include "GameFramework/Controller.h"
#include "PlayerController/ArcanePunkPlayerController.h"

void UAPStatusText::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayerPawn()->GetController());
    if(!PC) return;

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
    CharacterIndex->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().PlayerStaticData.MyCharacterIndex)));
    CharacterName->SetText(FText::FromName(Character->GetPlayerStatus().PlayerStaticData.CharacterName));
    CharacterGroup->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().PlayerStaticData.CharacterGroup)));
    Gender->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().PlayerStaticData.Gender)));
    CharacterRace->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().PlayerStaticData.CharacterRace)));
    CharacterType->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().PlayerStaticData.CharacterType)));
    HP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Character->GetPlayerStatus().PlayerDynamicData.HP))));
    MaxHP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Character->GetPlayerStatus().PlayerDynamicData.MaxHP))));
    MP->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().PlayerDynamicData.MP)));
    MaxMP->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().PlayerDynamicData.MaxMP)));
    ATK->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Character->GetFinalATK()))));
    ATKSpeed->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Character->GetPlayerStatus().PlayerDynamicData.ATKSpeed))));
    DEF->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Character->GetPlayerStatus().PlayerDynamicData.DEF))));
    MoveSpeed->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Character->GetPlayerStatus().PlayerDynamicData.MoveSpeed))));
    SP->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().PlayerDynamicData.SP)));
}