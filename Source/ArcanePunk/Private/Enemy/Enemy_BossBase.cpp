
#include "Enemy/Enemy_BossBase.h"

AEnemy_BossBase::AEnemy_BossBase()
{

}

void AEnemy_BossBase::BeginPlay()
{
    Super::BeginPlay();
    SetActorTickEnabled(true);
    InitPatternNums();
}

void AEnemy_BossBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(BossPhase == EBossPhase::Phase_1)
    {
        if(TotalStatus.StatusData.HP / TotalStatus.StatusData.MaxHP <= Phase2_HPPercent)
        BossPhase = EBossPhase::Phase_2;
    }
}

// Pattern Num Func Start
void AEnemy_BossBase::InitPatternNums()
{
    TMap<uint8, float> Percents;
    
    if(BossPhase == EBossPhase::Phase_1)
    {
        int index = 0;
        for(auto Pattern : Phase1_Pattern)
        Percents.Emplace(++index, Pattern.Value);
    }
    else if(BossPhase == EBossPhase::Phase_2)
    {
        int index = 0;
        for(auto Pattern : Phase2_Pattern)
        Percents.Emplace(++index, Pattern.Value);
    }

    int32 PercentNum = Percents.Num();
    while(PatternNums.Num() < PercentNum)
    {
        float Total = 0.0f;
        for(auto Percent : Percents)
        {
            Total += Percent.Value;
        }

        float Current = FMath::RandRange(0.0f, Total);

        uint8 Index = 0;
        for(auto Percent : Percents)
        {
            if(Current <= Percent.Value)
            {
                Index = Percent.Key;
                break;
            }
            Current -= Percent.Value;
        }
        PatternNums.Emplace(Index);
        Percents.Remove(Index);
    }

    for(auto V : PatternNums) UE_LOG(LogTemp, Display, TEXT("PatternNums %d"), V);
}

int32 AEnemy_BossBase::GetPatternNum()
{
    if(PatternNums.IsEmpty())
    {
        InitPatternNums();
    }

    int32 Num = PatternNums.Last(PatternNums.Num() - 1);
    PatternNums.Remove(Num);

    UE_LOG(LogTemp, Display, TEXT("Num %d"), Num);
    return Num;
}