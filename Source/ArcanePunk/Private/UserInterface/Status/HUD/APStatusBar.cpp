
#include "UserInterface/Status/HUD/APStatusBar.h"

#include "Components/ProgressBar.h"
#include "UserInterface/APHUD.h"
#include "Character/ArcanePunkCharacter.h"

void UAPStatusBar::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    InitStatusBar();
    
}

void UAPStatusBar::NativeConstruct()
{
    Super::NativeConstruct();
}

void UAPStatusBar::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if(bChange) UpdatePercentBar(InDeltaTime);
    
}

void UAPStatusBar::InitStatusBar()
{
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter) return;
    auto PD = OwnerCharacter->GetPlayerStatus();
    HUD = Cast<AAPHUD>(GetOwningPlayer()->GetHUD()); if(!HUD) return;

    HPBar->SetPercent(PD.PlayerDynamicData.HP / PD.PlayerDynamicData.MaxHP); OriginHP = PD.PlayerDynamicData.HP;
    MPBar->SetPercent(PD.PlayerDynamicData.MP / PD.PlayerDynamicData.MaxMP);

    HUD->OnUpdateHPBar.AddUObject(this, &UAPStatusBar::SetHPPercent);
}

void UAPStatusBar::SetHPPercent(float Origin)
{
    if(!OwnerCharacter) return; auto PD = OwnerCharacter->GetPlayerStatus();

    OriginHP = Origin;

    if(OriginHP != PD.PlayerDynamicData.HP) bChange = true;
}

void UAPStatusBar::UpdatePercentBar(float InDeltaTime)
{
    if(!OwnerCharacter) return; auto PD = OwnerCharacter->GetPlayerStatus();
    if(OriginHP == PD.PlayerDynamicData.HP) {bChange = false; return;}

    OriginHP = FMath::FInterpTo(OriginHP, PD.PlayerDynamicData.HP, InDeltaTime, BarSpeed);

    HPBar->SetPercent(OriginHP / PD.PlayerDynamicData.MaxHP);
    MPBar->SetPercent(PD.PlayerDynamicData.MP / PD.PlayerDynamicData.MaxMP);
}
