
#include "UserInterface/EnhanceUI/EnhanceGauge.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "PlayerController/ArcanePunkPlayerController.h"

void UEnhanceGauge::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    SetIsFocusable(true);
    SetKeyboardFocus();
    
    if(bTickable)
    {
        Current = FMath::FInterpConstantTo(Current, Target, InDeltaTime, Speed);
        Gauge_ProgressBar->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Percent"), Current);
        Target_Text->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Current * Max_Number))));
        if(abs(Current - Target) <= KINDA_SMALL_NUMBER)
        {
            bTickable = false;
            DisplayCompleteText();
        }
    }
}

void UEnhanceGauge::DisplayEnhanceGauge(int32 TargetNum, int32 MaxNum)
{
    SetIsFocusable(true);
    SetKeyboardFocus();
    Target = static_cast<float>(TargetNum) / static_cast<float>(MaxNum);
    Target_Number = TargetNum; Max_Number = MaxNum;
    Gauge_ProgressBar->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Percent"), 0.0f);
    Max_Text->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Max_Number))));

    OnGaugeDisplay();
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UEnhanceGauge::SetCustomTickEnable, Delay, false);
}

void UEnhanceGauge::SetCustomTickEnable()
{
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    bTickable = true;
}

void UEnhanceGauge::DisplayCompleteText()
{
    Target_Text->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Target_Number))));
    
    bCloseOn = true;
    if(Target_Number == Max_Number)
    {
        // 증강 모두 도달 텍스트, 애니메이션 활성화
        OnCompleteDisplay();
    }
    else
    {
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UEnhanceGauge::RemoveFromParent, Delay, false);
    }
}


FReply UEnhanceGauge::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent) // 클릭 아이템 인벤토리로 (오른쪽 마우스)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    if(!bCloseOn) Reply.Handled();
    RemoveFromParent();
    return Reply.Handled();
}
