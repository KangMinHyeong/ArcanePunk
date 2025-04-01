
#include "UserInterface/HUD/Status/Bar/ChangeCharacterUI.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UChangeCharacterUI::Init(uint8 KeyNum, UTexture2D* Portrait)
{
    Text_PlayerNum->SetText(FText::FromString(FString::FromInt(KeyNum)));
    PlayerImage->SetBrushFromTexture(Portrait);
}

void UChangeCharacterUI::ChangePlayer(bool IsPlayer)
{
    if(IsPlayer)
    {
        CurrentPlayerRing->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        CurrentPlayerRing->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UChangeCharacterUI::StartSwapCoolDown(float CoolTime)
{
    CoolDown = false;
    GetWorld()->GetTimerManager().SetTimer(ChangeTimerHandle, this, &UChangeCharacterUI::EndSwapCoolDown, CoolTime, false);
}

void UChangeCharacterUI::EndSwapCoolDown()
{
    CoolDown = true;
    GetWorld()->GetTimerManager().ClearTimer(ChangeTimerHandle);
}

bool UChangeCharacterUI::CheckSwapCoolDown()
{
    return false;
}
