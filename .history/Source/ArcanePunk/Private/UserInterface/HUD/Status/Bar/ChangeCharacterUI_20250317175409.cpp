
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
        if(CoolTime > 0.001f) {CoolTimePercent->SetVisibility(ESlateVisibility::Visible);}
        else {CoolTimePercent->SetVisibility(ESlateVisibility::Hidden);}
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
    GetWorld()->GetTimerManager().SetTimer(ChangeTimerHandle, this, &UChangeCharacterUI::EndSwapCoolDown, CoolTime, false);
}

void UChangeCharacterUI::EndSwapCoolDown()
{
    CoolDown = true;
    GetWorld()->GetTimerManager().ClearTimer(ChangeTimerHandle);
}

float UChangeCharacterUI::GetCurrentCoolTime()
{
	float CoolTime = CurrentCoolTime - AddCoolTime - (GetWorld()->GetTimerManager().GetTimerRate(SkillCoolTimerHandle) - GetWorld()->GetTimerManager().GetTimerElapsed(SkillCoolTimerHandle));
    return CoolTime;
}

bool UChangeCharacterUI::CheckSwapCoolDown()
{
    return CoolDown;
}
