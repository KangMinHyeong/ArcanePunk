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
    CharacterIndex->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().MyCharacterIndex)));
    CharacterName->SetText(FText::FromName(Character->GetPlayerStatus().CharacterName));
    CharacterGroup->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().CharacterGroup)));
    Gender->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().Gender)));
    CharacterRace->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().CharacterRace)));
    CharacterType->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().CharacterType)));
    HP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Character->GetPlayerStatus().HP))));
    MaxHP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Character->GetPlayerStatus().MaxHP))));
    MP->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().MP)));
    MaxMP->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().MaxMP)));
    ATK->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Character->GetFinalATK()))));
    ATKSpeed->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Character->GetPlayerStatus().ATKSpeed))));
    DEF->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Character->GetPlayerStatus().DEF))));
    MoveSpeed->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Character->GetPlayerStatus().MoveSpeed))));
    SP->SetText(FText::FromString(FString::FromInt(Character->GetPlayerStatus().SP)));
}