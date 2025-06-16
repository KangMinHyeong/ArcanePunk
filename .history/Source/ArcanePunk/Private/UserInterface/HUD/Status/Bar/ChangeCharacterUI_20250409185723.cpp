
#include "UserInterface/HUD/Status/Bar/ChangeCharacterUI.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UChangeCharacterUI::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick( MyGeometry, InDeltaTime);

    if(!CoolDown) UpdateSwapCoolTime();
    UpdateSwapGauge(InDeltaTime);
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
        Border_Gauge->SetVisibility(ESlateVisibility::Collapsed);
        Border_Number->SetVisibility(ESlateVisibility::Collapsed);
    }
    else
    {
        CurrentPlayerRing->SetVisibility(ESlateVisibility::Collapsed);
        Border_Gauge->SetVisibility(ESlateVisibility::Visible);
        Border_Number->SetVisibility(ESlateVisibility::Visible);
    }
}

void UChangeCharacterUI::StartSwapCoolDown(float CoolTime)
{
    SwapCoolTime = CoolTime;
    CoolDown = false;
    CoolTimePercent->SetVisibility(ESlateVisibility::Visible);
    CoolTimeText->SetVisibility(ESlateVisibility::Visible);

    CoolTimePercent->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Percent"), 1.0f);
    CoolTimeText->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CoolTime))));

    GetWorld()->GetTimerManager().SetTimer(ChangeTimerHandle, this, &UChangeCharacterUI::EndSwapCoolDown, CoolTime, false);
}

void UChangeCharacterUI::EndSwapCoolDown()
{
    CoolDown = true;
    CoolTimePercent->SetVisibility(ESlateVisibility::Collapsed);
    CoolTimeText->SetVisibility(ESlateVisibility::Collapsed);

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

float UChangeCharacterUI::UpdateSwapGauge(float Add)
{
    CurrentSwapGaugeSpeed = SwapGaugeSpeed * (1.0f + (Add / 20.0f));
    TargetSwapGauge = FMath::Min(100.0f, CurrentSwapGauge + Add);
    return TargetSwapGauge;
}

void UChangeCharacterUI::UpdateSwapCoolTime()
{
    float CoolTime = SwapCoolTime - GetCurrentCoolTime();
    if(CoolTime > 1.0f) {CoolTimeText->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CoolTime))));}
    else {CoolTimeText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), CoolTime)));}

    float CoolPercent = GetCurrentCoolTime() / SwapCoolTime;
    CoolTimePercent->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Percent"), CoolPercent);
}
