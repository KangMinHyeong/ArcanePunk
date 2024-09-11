
#include "UserInterface/HUD/Status/Bar/APDashSlot.h"

#include "Components/ProgressBar.h"
#include "Character/ArcanePunkCharacter.h"

void UAPDashSlot::NativeConstruct()
{
    Super::NativeConstruct();
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter.IsValid()) return;
}

void UAPDashSlot::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if(bDashing)
    {
        float Percent = GetCurrentDashTime() / CurrentDashTime;
        if(Percent > 0.0001f) 
        {
            Skilling_ProgressBar->SetVisibility(ESlateVisibility::Visible);
            Skilling_ProgressBar->SetPercent(Percent);
        }
        else 
        {
            Skilling_ProgressBar->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UAPDashSlot::StartDashTime(float DashTime)
{
    bDashing = true;
    CurrentDashTime = DashTime;
    GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UAPDashSlot::EndDash, DashTime, false);
}

void UAPDashSlot::EndDash()
{
    GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
    bDashing = false;
    Skilling_ProgressBar->SetVisibility(ESlateVisibility::Hidden);

    auto Player = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn());
    if(Player) Player->ReleasedDash();
}

float UAPDashSlot::GetCurrentDashTime()
{
    float Time = (GetWorld()->GetTimerManager().GetTimerRate(DashTimerHandle) - GetWorld()->GetTimerManager().GetTimerElapsed(DashTimerHandle));
    return Time;
}