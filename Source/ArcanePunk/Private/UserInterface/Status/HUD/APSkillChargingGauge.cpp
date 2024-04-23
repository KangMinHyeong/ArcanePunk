
#include "UserInterface/Status/HUD/APSkillChargingGauge.h"

#include "Components/ProgressBar.h"

void UAPSkillChargingGauge::NativeConstruct()
{
    Super::NativeConstruct();
    SetVisibility(ESlateVisibility::Collapsed);
}

void UAPSkillChargingGauge::SetSkillChargingPercent(float Current, float Max)
{
    ChargingGauge->SetPercent(Current/Max);
}

void UAPSkillChargingGauge::ChargeEnd(bool Start)
{
    if(Start)
    {
        SetVisibility(ESlateVisibility::Visible);
        ChargingGauge->SetPercent(0.0f);
    }
    else
    {
        SetVisibility(ESlateVisibility::Collapsed);
    }
}