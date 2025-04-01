
#include "UserInterface/HUD/Status/Bar/APHPBar.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "UserInterface/Common/APShieldHPBar.h"

void UAPHPBar::NativeConstruct()
{
    Super::NativeConstruct();

    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter.IsValid()) return;
    const auto PD = OwnerCharacter->GetPlayerStatus();
    
    HPBar->SetPercent(PD.StatusData.HP / PD.StatusData.MaxHP); OriginHP = PD.StatusData.HP;
    TEXT_CurrentMaxHP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(PD.StatusData.MaxHP))));
    TEXT_CurrentHP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(OriginHP))));

    OwnerCharacter->OnCheckingShield.RemoveDynamic(ShieldHPBar, &UAPShieldHPBar::CheckShieldHP);
    OwnerCharacter->OnCheckingShield.AddDynamic(ShieldHPBar, &UAPShieldHPBar::CheckShieldHP);
}

void UAPHPBar::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if(bChange) UpdatePercentBar(InDeltaTime);
    
}

void UAPHPBar::SetMaxHP(float NewValue)
{
    if(!OwnerCharacter.IsValid()) return;
    
    const auto PD = OwnerCharacter->GetPlayerStatus();
    TEXT_CurrentMaxHP->SetText(FText::FromString(FString::FromInt(round(PD.StatusData.MaxHP))));
    TEXT_CurrentHP->SetText(FText::FromString(FString::FromInt(round(PD.StatusData.HP))));
    HPBar->SetPercent(PD.StatusData.HP / PD.StatusData.MaxHP);
}

void UAPHPBar::SetHPPercent(AArcanePunkCharacter* Character, float Origin)
{
    if(!OwnerCharacter.IsValid()) return; const auto PD = OwnerCharacter->GetPlayerStatus();

    OriginHP = Origin;
    bChange = true;
}

void UAPHPBar::UpdatePercentBar(float InDeltaTime)
{
    if(!OwnerCharacter.IsValid()) return; const auto PD = OwnerCharacter->GetPlayerStatus();

    OriginHP = FMath::FInterpConstantTo(OriginHP, PD.StatusData.HP, InDeltaTime, BarSpeed);
    HPBar->SetPercent(OriginHP / PD.StatusData.MaxHP);
    TEXT_CurrentHP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(OriginHP))));
}
