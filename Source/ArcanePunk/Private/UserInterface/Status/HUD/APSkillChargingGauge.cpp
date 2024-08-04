
#include "UserInterface/Status/HUD/APSkillChargingGauge.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameInstance/APGameInstance.h"
#include "Interfaces/InteractionInterface.h"

void UAPSkillChargingGauge::NativeConstruct()
{
    Super::NativeConstruct();
    SetVisibility(ESlateVisibility::Collapsed);

    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;  
    auto DataTable = APGI->GetStringData()->FindRow<FStringDataTable>(Charging, Charging.ToString()); if(!DataTable) return;
    Text_Charging->SetText(FText::FromString(DataTable->Content));
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