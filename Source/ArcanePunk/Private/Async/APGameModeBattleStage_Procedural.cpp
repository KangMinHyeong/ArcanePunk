// Fill out your copyright notice in the Description page of Project Settings.
//
// APGameModeBattleStage의 절차적 레벨 생성 멀티스레드 구현
// TryGenerateLevelPlan: Thread-safe한 단일 시도 레벨 생성
// SelectBestPlan: 여러 후보 중 최적 결과 선택

#include "GameMode/APGameModeBattleStage.h"
#include "Math/RandomStream.h"

FGenerationPlan AAPGameModeBattleStage::TryGenerateLevelPlan(
    int32 AttemptIndex, int32& OutMainLevelsGenerated,
    const TArray<FLevelModuleData>& InLevelModules,
    const FLevelModuleData& InStartLevel,
    const FLevelModuleData& InShopLevel,
    int32 InTargetLevels, int32 InMaxDist, int32 InMaxPathDist,
    int32 InTotalMaxDist, FIntPoint InBossOffset,
    float InRoomSpacing, bool bInFirstHidden)
{
    // Thread-safe를 위해 모든 상태를 로컬 변수로 관리
    TSet<FIntPoint> LocalUsed;
    TMap<FIntPoint, int32> LocalTileNumber;
    FGenerationPlan Plan;
    TArray<FGeneratedLevelModuleInfo> GeneratedLevelModuleInfo;

    OutMainLevelsGenerated = 0;
    const int32 MaxMainLoopSteps = 2000;

    // 시드를 AttemptIndex 기반으로 설정 (재현 가능하게)
    FRandomStream RandomStream(AttemptIndex);

    // === 메인 경로 생성 ===
    {
        TArray<FIntPoint> Directions = { {0,1}, {0,-1}, {-1,0}, {1,0} };

        // StartLevel 설정
        TArray<TPair<FIntPoint, ELevelDirection>> ExitArray;
        for (const auto& Exit : InStartLevel.LevelExit)
        {
            ExitArray.Add(Exit);
        }
        auto LevelExit = ExitArray[RandomStream.RandRange(0, ExitArray.Num() - 1)];

        FGeneratedLevelModuleInfo StartInfo;
        StartInfo.BaseNode = LevelExit.Key;
        StartInfo.LevelModuleData = InStartLevel;
        StartInfo.LevelNumber = 1;
        StartInfo.Used.Add(StartInfo.BaseNode);
        StartInfo.Maps.FindOrAdd(StartInfo.LevelNumber).Add({StartInfo.BaseNode});
        StartInfo.TileNumber.Add({StartInfo.BaseNode, StartInfo.LevelNumber});
        GeneratedLevelModuleInfo.Add(StartInfo);

        int32 MainLoopSteps = 0;

        while (true)
        {
            if (++MainLoopSteps > MaxMainLoopSteps)
            {
                break; // 실패
            }

            if (GeneratedLevelModuleInfo.Num() == 0)
            {
                GeneratedLevelModuleInfo.Add(StartInfo);
                continue;
            }

            const auto& LastData = GeneratedLevelModuleInfo.Last();
            const auto& BaseNode = LastData.BaseNode;
            const auto& ModuleData = LastData.LevelModuleData;

            // 랜덤 Exit 선택
            TArray<TPair<FIntPoint, ELevelDirection>> ExitsArray;
            for (const auto& Exit : ModuleData.LevelExit)
            {
                ExitsArray.Add(Exit);
            }
            auto LastLevelExit2 = ExitsArray[RandomStream.RandRange(0, ExitsArray.Num() - 1)];

            const auto& LastNode = BaseNode + LastLevelExit2.Key;
            auto LastDir = Directions[(uint8)LastLevelExit2.Value];
            LastDir = FIntPoint(-LastDir.X, -LastDir.Y);

            if (LastData.LevelNumber >= InTargetLevels)
            {
                OutMainLevelsGenerated = LastData.LevelNumber;
                break; // 성공
            }

            LocalUsed = LastData.Used;
            LocalTileNumber = LastData.TileNumber;

            // 랜덤 레벨 모듈 선택
            FLevelModuleData CurrentLevelData = InLevelModules[RandomStream.RandRange(0, InLevelModules.Num() - 1)];
            FIntPoint Shape = CurrentLevelData.LevelShape;

            FIntPoint CurrentNode;
            CurrentNode = LastNode + FIntPoint(
                RandomStream.RandRange(-InMaxDist, InMaxDist),
                RandomStream.RandRange(-InMaxDist, InMaxDist)
            );

            // 이전 노드보다 앞이면 재시도
            if (CurrentNode.X + CurrentNode.Y <= LastNode.X + LastNode.Y)
            {
                GeneratedLevelModuleInfo.Pop();
                continue;
            }

            TArray<FIntPoint> FinalRoute;
            TSet<FIntPoint> Occupied;
            TMap<uint8, TSet<FIntPoint>> Maps = LastData.Maps;
            auto LevelNumber = LastData.LevelNumber;
            auto TotalLevelDist = LastData.TotalLevelDist;

            if (!CanPlaceRoom(CurrentNode, Shape, LocalUsed, Occupied, InBossOffset))
            {
                GeneratedLevelModuleInfo.Pop();
                continue;
            }

            FinalRoute = FindPathBetweenNodes(CurrentLevelData, CurrentNode, LastDir, Directions,
                                              LevelNumber, Occupied, LocalUsed, LocalTileNumber);
            if (FinalRoute.IsEmpty())
            {
                GeneratedLevelModuleInfo.Pop();
                continue;
            }

            LevelNumber++;

            if (LevelNumber > InTargetLevels)
            {
                OutMainLevelsGenerated = InTargetLevels;
                break; // 성공
            }

            TotalLevelDist += FinalRoute.Num();
            TotalLevelDist += (Shape.X + Shape.Y) / 2;
            if (TotalLevelDist > InTotalMaxDist)
            {
                break;
            }

            TSet<FIntPoint>& TargetSet = Maps.FindOrAdd(LevelNumber);
            for (const FIntPoint& P : Occupied)
            {
                TargetSet.Add(P);
                LocalTileNumber.Add({P, LevelNumber});
                LocalUsed.Add(P);
            }
            for (FIntPoint P : FinalRoute)
            {
                LocalUsed.Add(P);
            }

            // 계획 추가
            {
                FVector LevelLocation = CalculateLevelLocation(CurrentNode, Shape, InRoomSpacing);
                FPlannedSubLevel R;
                R.LevelPath = CurrentLevelData.LevelWorld.ToSoftObjectPath().ToString();
                R.Location = LevelLocation;
                R.Rotation = FRotator::ZeroRotator;
                R.bInitiallyVisible = !bInFirstHidden;
                Plan.Rooms.Add(R);

                Plan.Triggers.Add(MakeTuple(LevelLocation, Shape));
                PlanPassagesAlongRoute(FinalRoute, Plan);
            }

            if (Plan.Rooms.Num() >= InTargetLevels)
            {
                OutMainLevelsGenerated = InTargetLevels;
                break; // 성공
            }

            // push
            FGeneratedLevelModuleInfo LevelModuleInfo;
            LevelModuleInfo.BaseNode = CurrentNode;
            LevelModuleInfo.LevelModuleData = CurrentLevelData;
            LevelModuleInfo.Paths = FinalRoute;
            LevelModuleInfo.TotalLevelDist = TotalLevelDist;
            LevelModuleInfo.LevelNumber = LevelNumber;
            LevelModuleInfo.Used = LocalUsed;
            LevelModuleInfo.TileNumber = LocalTileNumber;
            LevelModuleInfo.Maps = Maps;
            GeneratedLevelModuleInfo.Add(LevelModuleInfo);
        }

        if (GeneratedLevelModuleInfo.Num() > 0)
        {
            OutMainLevelsGenerated = FMath::Min(InTargetLevels, GeneratedLevelModuleInfo.Last().LevelNumber);
        }
    }

    // [CHANGED] 목표 미달이어도 부분 결과 반환 (SelectBestPlan에서 최적 선택)
    // 멀티스레드 환경에서는 일부 스레드라도 부분 성공할 수 있으므로
    // 빈 플랜보다는 부분 결과가 낫습니다
    if (OutMainLevelsGenerated == 0 || Plan.Rooms.Num() == 0)
    {
        return FGenerationPlan(); // 완전 실패만 빈 플랜 반환
    }

    // === 상점 삽입 로직 ===
    {
        const int32 ShopCount = 2;
        const int32 TotalLevelsForShops = OutMainLevelsGenerated;

        if (TotalLevelsForShops >= 2 && ShopCount > 0 && GeneratedLevelModuleInfo.Num() > 0)
        {
            const int32 Step = FMath::Max(1, InTargetLevels / (ShopCount + 1));
            TArray<TPair<int32,int32>> ShopPairs;

            for (int32 i = 1; i <= ShopCount; ++i)
            {
                const int32 k = i * Step;
                const int32 a = FMath::Clamp(k,     1, TotalLevelsForShops - 1);
                const int32 b = FMath::Clamp(k + 1, 2, TotalLevelsForShops);
                if (GeneratedLevelModuleInfo.IsValidIndex(a) && GeneratedLevelModuleInfo.IsValidIndex(b))
                {
                    ShopPairs.Emplace(a, b);
                }
            }

            for (const auto& Pair : ShopPairs)
            {
                const int32 LevelA = Pair.Key;
                const int32 LevelB = Pair.Value;

                if (!GeneratedLevelModuleInfo.IsValidIndex(LevelA) ||
                    !GeneratedLevelModuleInfo.IsValidIndex(LevelB))
                    continue;

                const TSet<FIntPoint>& TilesA = GeneratedLevelModuleInfo[LevelA].Maps[LevelA];
                if (TilesA.Num() == 0) continue;

                TArray<FIntPoint> TilesArrA = TilesA.Array();
                FIntPoint BaseFromA = TilesArrA[RandomStream.RandRange(0, TilesArrA.Num() - 1)];

                FIntPoint CandidateNode = BaseFromA + FIntPoint(
                    RandomStream.RandRange(-InMaxDist, InMaxDist),
                    RandomStream.RandRange(-InMaxDist, InMaxDist)
                );

                TSet<FIntPoint> ShopOccupied;
                if (CanPlaceRoom(CandidateNode, InShopLevel.LevelShape, LocalUsed, ShopOccupied, InBossOffset))
                {
                    for (const FIntPoint& P : ShopOccupied) LocalUsed.Add(P);

                    FVector ShopLocation = CalculateLevelLocation(CandidateNode, InShopLevel.LevelShape, InRoomSpacing);
                    FPlannedSubLevel R;
                    R.LevelPath = InShopLevel.LevelWorld.ToSoftObjectPath().ToString();
                    R.Location = ShopLocation;
                    R.Rotation = FRotator::ZeroRotator;
                    R.bInitiallyVisible = !bInFirstHidden;
                    Plan.Rooms.Add(R);
                    Plan.Triggers.Add(MakeTuple(ShopLocation, InShopLevel.LevelShape));
                }
            }
        }
    }

    return Plan;
}

FGenerationPlan AAPGameModeBattleStage::SelectBestPlan(const TArray<FGenerationPlan>& Candidates,
                                                       const TArray<int32>& MainLevelCounts)
{
    int32 BestIndex = -1;
    int32 BestCount = 0;
    int32 BestTotalRooms = 0;

    for (int32 i = 0; i < Candidates.Num(); ++i)
    {
        const int32 CurrentMainCount = MainLevelCounts[i];
        const int32 CurrentTotalRooms = Candidates[i].Rooms.Num();

        // 1순위: 메인 레벨 개수가 목표에 가까운 것
        // 2순위: 총 방 개수가 많은 것 (상점 포함)
        bool bIsBetter = false;

        if (CurrentMainCount > BestCount)
        {
            bIsBetter = true;
        }
        else if (CurrentMainCount == BestCount && CurrentTotalRooms > BestTotalRooms)
        {
            bIsBetter = true;
        }

        if (bIsBetter)
        {
            BestIndex = i;
            BestCount = CurrentMainCount;
            BestTotalRooms = CurrentTotalRooms;
        }
    }

    if (BestIndex >= 0 && Candidates.IsValidIndex(BestIndex))
    {
        UE_LOG(LogTemp, Display, TEXT("[SelectBestPlan] Selected Plan: %d main levels, %d total rooms"),
               BestCount, BestTotalRooms);
        return Candidates[BestIndex];
    }

    UE_LOG(LogTemp, Error, TEXT("[SelectBestPlan] No valid plan found!"));
    return FGenerationPlan(); // 빈 플랜
}
