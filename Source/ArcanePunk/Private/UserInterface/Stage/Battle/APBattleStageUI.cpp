
#include "UserInterface/Stage/Battle/APBattleStageUI.h"

#include "Components/TextBlock.h"
#include "GameInstance/APGameInstance.h"

void UAPBattleStageUI::NativeConstruct()
{
    Super::NativeConstruct();

    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;  
    APGI->SetTextBlock(TEXT_RemainWave, EStringRowName::RemainWave);
    APGI->SetTextBlock(TEXT_NexWaveTime, EStringRowName::RemainWaveTime);
    
}

void UAPBattleStageUI::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick( MyGeometry, InDeltaTime);

    float WaveTime = GetWaveTime();

    WaveTime = CurrentWaveTime - WaveTime;
    if(WaveTime > 1.0f) {TEXT_CurrentWaveTime->SetText(FText::FromString(FString::FromInt(static_cast<int32>(WaveTime))));}
    else {TEXT_CurrentWaveTime->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), WaveTime)));}

    // if(WaveTime < 0.1f) TEXT_CurrentWaveTime->SetText(FText::FromString(FString::FromInt(RemainWave)))
}

void UAPBattleStageUI::UpdateBattleUI(int32 RemainWave, float WaveTime)
{
    GetWorld()->GetTimerManager().ClearTimer(SpawnMonsterTimerHandle);
    TEXT_RemainWaveNum->SetText(FText::FromString(FString::FromInt(RemainWave)));
    if(RemainWave == 0) return;
    GetWorld()->GetTimerManager().SetTimer(SpawnMonsterTimerHandle, this, &UAPBattleStageUI::StartNewWave, WaveTime, false);
    CurrentWaveTime = WaveTime;
}

void UAPBattleStageUI::StartNewWave()
{
    OnStartingNewWave.Broadcast();
}

float UAPBattleStageUI::GetWaveTime()
{
    float WaveTime = CurrentWaveTime - (GetWorld()->GetTimerManager().GetTimerRate(SpawnMonsterTimerHandle) - GetWorld()->GetTimerManager().GetTimerElapsed(SpawnMonsterTimerHandle));
    return WaveTime;
}
