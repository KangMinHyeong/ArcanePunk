
#include "UserInterface/HUD/Status/Bar/ChangeCharacterUI.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UChangeCharacterUI::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick( MyGeometry, InDeltaTime);

    if(CoolDown)
    {

    }
    else
    {
        float CoolTime = GetCurrentCoolTime() / SkillCoolTime;
        CoolTimePercent->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Percent"), CoolTime);
    }
}

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
    SwapCoolTime = CoolTime;
    CoolDown = false;
    CoolTimePercent->SetVisibility(ESlateVisibility::Hidden);
    
    GetWorld()->GetTimerManager().SetTimer(ChangeTimerHandle, this, &UChangeCharacterUI::EndSwapCoolDown, CoolTime, false);
}

void UChangeCharacterUI::EndSwapCoolDown()
{
    CoolDown = true;
    GetWorld()->GetTimerManager().ClearTimer(ChangeTimerHandle);
}

float UChangeCharacterUI::GetCurrentCoolTime()
{
	float CoolTime = SwapCoolTime - (GetWorld()->GetTimerManager().GetTimerRate(ChangeTimerHandle) - GetWorld()->GetTimerManager().GetTimerElapsed(ChangeTimerHandle));
    return CoolTime;
}

bool UChangeCharacterUI::CheckSwapCoolDown()
{
    return CoolDown;
}
