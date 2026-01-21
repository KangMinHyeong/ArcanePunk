
#include "GameMode/APGameModeBattleStage.h"

#include "GameElements/Volume/SpawnVolume/APSpawnVolume.h"
#include "EngineUtils.h"
#include "ArcanePunk/Public/Enemy/Enemy_CharacterBase.h"
#include "GameState/APGameState.h"
#include "GameFramework/Controller.h"
#include "AIController.h"
#include "GameElements/Trap/APTrapBase.h"
#include "Enemy/Drop/Enemy_DropPackage.h"
#include "GameElements/Portal/Portal_Base.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "GameInstance/APGameInstance.h"
#include "Engine/LevelStreamingDynamic.h"
#include "LevelStream/APLevelStreamTrigger.h"
#include "GameInstance/APGameInstance.h"

void AAPGameModeBattleStage::PlayerKilled()
{
    for(AController* Controller : TActorRange<AController>(GetWorld()))
    {
        auto AIC = Cast<AAIController>(Controller); if(!AIC) continue;
        AIC->GameHasEnded(Controller->GetPawn(), false);
        AIC->UnPossess();
    } 
}

void AAPGameModeBattleStage::CheckClearStage(AActor* SpawnVolume)
{
    auto Spawner = Cast<AAPSpawnVolume>(SpawnVolume); if(!Spawner) return;

    // 몬스터가 다 처지 되었으면 호출, 배틀섹션에서 추가로 몬스터를 스폰할껀지 확인, 확인해서 배틀섹션 종료 및 지속 결정
	// if(Spawner->CheckSpawnEnd())
    // {
    //     // for(auto Trap : TActorRange<AAPTrapBase>(GetWorld()))
    //     //     Trap->Deactivate();

    //     // GetWorld()->GetWorldSettings()->SetTimeDilation(0.25f);
    //     // FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AAPGameModeBattleStage::PortalSpawn, Spawner->GetSpawnDataID());
	//     // GetWorld()->GetTimerManager().SetTimer(PortalSpawnTimerHandle, TimerDelegate, 0.65f, false);
    // }
}

void AAPGameModeBattleStage::PortalSpawn(FName CurrentClearStage)
{
    GetWorld()->GetTimerManager().ClearTimer(PortalSpawnTimerHandle);
    GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
    
    bool Check = false;
    for(auto Drop : TActorRange<AEnemy_DropPackage>(GetWorld()))
    {
        if(CurrentClearStage == Drop->GetDropID())
        {
            Check = true;
            Drop->DropActivate();
        }
    }

    if(Check) return;
    for(auto Portal : TActorRange<APortal_Base>(GetWorld()))
    {
        if(CurrentClearStage == Portal->GetPortalID())
        {
            Portal->InitHide(false);
        }
    }
}

void AAPGameModeBattleStage::OnEndedGame()
{
    auto PC = Cast<AArcanePunkPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if(PC) PC->DisplayEnding();
}

void AAPGameModeBattleStage::StartPlay()
{
	Super::StartPlay();

    if (bUseAsyncLevelGeneration)
    {
        // [NEW] 비동기 멀티스레드 버전
        UE_LOG(LogTemp, Warning, TEXT("========== Using ASYNC Level Generation =========="));
        CreateLevelAsync();
    }
    else
    {
        // [OLD] 동기 버전 (벤치마크 비교용)
        UE_LOG(LogTemp, Warning, TEXT("========== Using SYNC Level Generation =========="));
        CreateLevel();
    }
}

void AAPGameModeBattleStage::CreateLevel()
{
    // [SYNC] 벤치마크 시작
    const double SyncStartTime = FPlatformTime::Seconds();

    const int32 MaxGlobalAttempts   = 500;
    const int32 MaxMainLoopSteps    = 2000;

    for (int32 GlobalAttempt = 0; GlobalAttempt < MaxGlobalAttempts; ++GlobalAttempt)
    {
        bool bSuccessful = false;
        bool bAnySpawnedThisAttempt = false; // 논리적 생성 여부 (Used 업데이트/Plan에 누적)

        // 시도별 상태 초기화
        Used.Empty();
        TileNumber.Empty();

        FGenerationPlan Plan; // 이번 시도에서 생성할 모든 방/통로/트리거 계획

        TArray<FGeneratedLevelModuleInfo> FinalMapInfos;
        TArray<FGeneratedLevelModuleInfo> GeneratedLevelModuleInfo;

        int32 CurrentMainLevels = 0; // 이번 시도에서 실제 생성된 메인 방 수

        // === 메인 경로 생성 ===
        {
            TArray<FIntPoint> Directions = { {0,1}, {0,-1}, {-1,0}, {1,0} };
            auto LevelExit = GetRandomElement(StartLevelModuleData.LevelExit);

            FGeneratedLevelModuleInfo StartInfo;
            StartInfo.BaseNode = LevelExit.Key;
            StartInfo.LevelModuleData = StartLevelModuleData;
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
                    bSuccessful = false;
                    break;
                }

                // [FIX] 비어 있으면 즉시 재시도 or break
                if (GeneratedLevelModuleInfo.Num() == 0)
                {
                    GeneratedLevelModuleInfo.Add(StartInfo);
                    continue;
                }

                const auto& LastData = GeneratedLevelModuleInfo.Last();
                const auto& BaseNode = LastData.BaseNode;

                const auto& ModuleData = LastData.LevelModuleData;
                const auto& LastLevelExit2 = GetRandomElement(ModuleData.LevelExit);
                const auto& LastNode = BaseNode + LastLevelExit2.Key;
                auto LastDir = Directions[(uint8)LastLevelExit2.Value];
                LastDir = FIntPoint(-LastDir.X, -LastDir.Y);

                // [수정] 목표 방 개수 초과 체크 먼저
                if (LastData.LevelNumber >= TargetMainLevels)
                {
                    bSuccessful = true;
                    CurrentMainLevels = LastData.LevelNumber;
                    break;
                }

                Used = LastData.Used;
                TileNumber = LastData.TileNumber;

                FLevelModuleData CurrentLevelData = GetRandomElement(LevelModuleData);
                FIntPoint Shape = CurrentLevelData.LevelShape;

                FIntPoint CurrentNode;
                TryGenerateNextRoomCandidate(LastNode, CurrentNode);

                TArray<FIntPoint> FinalRoute;
                TSet<FIntPoint> Occupied;
                TMap<uint8, TSet<FIntPoint>> Maps = LastData.Maps;
                auto LevelNumber = LastData.LevelNumber;
                auto TotalLevelDist = LastData.TotalLevelDist;

                if (!CanPlaceRoom(CurrentNode, Shape, Used, Occupied, BossLevelOffset))
                {
                    GeneratedLevelModuleInfo.Pop();
                    continue;
                }

                FinalRoute = FindPathBetweenNodes(CurrentLevelData, CurrentNode, LastDir, Directions,
                                                 LevelNumber, Occupied, Used, TileNumber);
                if (FinalRoute.IsEmpty())
                {
                    GeneratedLevelModuleInfo.Pop();
                    continue;
                }

                // [수정] LevelNumber 즉시 증가
                LevelNumber++;

                // [수정] 목표 방 개수 도달 시 강제 종료
                if (LevelNumber > TargetMainLevels)
                {
                    bSuccessful = true;
                    CurrentMainLevels = TargetMainLevels;
                    break;
                }

                TotalLevelDist += FinalRoute.Num();
                TotalLevelDist += (Shape.X + Shape.Y) / 2;
                if (TotalLevelDist > TotalMaxLevelDist)
                {
                    break;
                }

                TSet<FIntPoint>& TargetSet = Maps.FindOrAdd(LevelNumber);
                for (const FIntPoint& P : Occupied)
                {
                    TargetSet.Add(P);
                    TileNumber.Add({P, LevelNumber});
                    Used.Add(P);
                }
                for (FIntPoint P : FinalRoute)
                {
                    Used.Add(P);
                }

                // 계획 추가
                {
                    FVector LevelLocation = CalculateLevelLocation(CurrentNode, Shape, RoomSpacing);
                    FPlannedSubLevel R;
                    R.LevelPath = CurrentLevelData.LevelWorld.ToSoftObjectPath().ToString();
                    R.Location = LevelLocation;
                    R.Rotation = FRotator::ZeroRotator;
                    R.bInitiallyVisible = !bFirstHidden;
                    Plan.Rooms.Add(R);

                    Plan.Triggers.Add(MakeTuple(LevelLocation, Shape));
                    PlanPassagesAlongRoute(FinalRoute, Plan);
                }

                bAnySpawnedThisAttempt = true;

                // [수정] Rooms 개수 제한
                if (Plan.Rooms.Num() >= TargetMainLevels)
                {
                    bSuccessful = true;
                    CurrentMainLevels = TargetMainLevels;
                    break;
                }

                // push
                FGeneratedLevelModuleInfo LevelModuleInfo;
                LevelModuleInfo.BaseNode = CurrentNode;
                LevelModuleInfo.LevelModuleData = CurrentLevelData;
                LevelModuleInfo.Paths = FinalRoute;
                LevelModuleInfo.TotalLevelDist = TotalLevelDist;
                LevelModuleInfo.LevelNumber = LevelNumber;
                LevelModuleInfo.Used = Used;
                LevelModuleInfo.TileNumber = TileNumber;
                LevelModuleInfo.Maps = Maps;
                GeneratedLevelModuleInfo.Add(LevelModuleInfo);
            }

            if (GeneratedLevelModuleInfo.Num() > 0)
            {
                CurrentMainLevels = FMath::Min(TargetMainLevels, GeneratedLevelModuleInfo.Last().LevelNumber);
            }
        }

        FinalMapInfos = GeneratedLevelModuleInfo;

        // === 상점 삽입 (계획만 추가, TileNumber/Maps 기록 X) ===
        {
            const int32 ShopCount = 2; // 프로젝트 옵션화 권장
            // [FIX] 상점 위치 계산은 "목표/실제 메인 방 수" 기준으로
            const int32 TotalLevelsForShops = CurrentMainLevels;
            if (TotalLevelsForShops >= 2 && ShopCount > 0)
            {
                const int32 Step = FMath::Max(1, TargetMainLevels / (ShopCount + 1)); // 10/3=3 → 3,6
                TArray<TPair<int32,int32>> ShopPairs;

                for (int32 i = 1; i <= ShopCount; ++i)
                {
                    const int32 k = i * Step; // 3, 6, ...
                    const int32 a = FMath::Clamp(k,     1, TotalLevelsForShops - 1);
                    const int32 b = FMath::Clamp(k + 1, 2, TotalLevelsForShops);
                    if (GeneratedLevelModuleInfo.IsValidIndex(a) && GeneratedLevelModuleInfo.IsValidIndex(b))
                    {
                        ShopPairs.Emplace(a, b);
                    }
                }

                auto TryPlanShopBetweenLevels = [&](int32 LevelA, int32 LevelB)->bool
                {
                    if (!GeneratedLevelModuleInfo.IsValidIndex(LevelA) || !GeneratedLevelModuleInfo.IsValidIndex(LevelB))
                        return false;

                    const FLevelModuleData& ShopData = ShopLevelModuleData; // 헤더에 선언 필요
                    const FIntPoint ShopShape = ShopData.LevelShape;

                    const TSet<FIntPoint>& TilesA = GeneratedLevelModuleInfo[LevelA].Maps[LevelA];
                    const TSet<FIntPoint>& TilesB = GeneratedLevelModuleInfo[LevelB].Maps[LevelB];
                    if (TilesA.Num() == 0 || TilesB.Num() == 0) return false;

                    TArray<FIntPoint> TilesArrA = TilesA.Array();

                    const int32 MaxAttempts = 64;
                    for (int32 Attempt = 0; Attempt < MaxAttempts; ++Attempt)
                    {
                        FIntPoint BaseFromA = TilesArrA[FMath::RandRange(0, TilesArrA.Num() - 1)];
                        FIntPoint CandidateNode;
                        TryGenerateNextRoomCandidate(BaseFromA, CandidateNode);

                        TSet<FIntPoint> ShopOccupied;
                        if (!CanPlaceRoom(CandidateNode, ShopShape, Used, ShopOccupied, BossLevelOffset)) continue;

                        auto TryFindPathToLevel = [&](int32 TargetLevel, TArray<FIntPoint>& OutRoute)->bool
                        {
                            for (const FIntPoint& LastDirTry : TArray<FIntPoint>{ FIntPoint(0,1), FIntPoint(0,-1), FIntPoint(-1,0), FIntPoint(1,0) })
                            {
                                TArray<FIntPoint> Tmp = FindPathBetweenNodes(
                                    ShopData, CandidateNode, LastDirTry,
                                    TArray<FIntPoint>{ FIntPoint(0,1), FIntPoint(0,-1), FIntPoint(-1,0), FIntPoint(1,0) },
                                    TargetLevel, ShopOccupied, Used, TileNumber
                                );
                                if (!Tmp.IsEmpty())
                                {
                                    OutRoute = MoveTemp(Tmp);
                                    return true;
                                }
                            }
                            return false;
                        };

                        TArray<FIntPoint> RouteToA, RouteToB;
                        if (!TryFindPathToLevel(LevelA, RouteToA)) continue;
                        if (!TryFindPathToLevel(LevelB, RouteToB)) continue;

                        // Used만 갱신(상점은 TileNumber/Maps 기록 X)
                        for (const FIntPoint& P : ShopOccupied) Used.Add(P);
                        for (const FIntPoint& P : RouteToA)     Used.Add(P);
                        for (const FIntPoint& P : RouteToB)     Used.Add(P);

                        // 계획 추가: 상점 방
                        {
                            FVector ShopLocation = CalculateLevelLocation(CandidateNode, ShopShape, RoomSpacing);
                            FPlannedSubLevel R;
                            R.LevelPath = ShopData.LevelWorld.ToSoftObjectPath().ToString();
                            R.Location = ShopLocation;
                            R.Rotation = FRotator::ZeroRotator;
                            R.bInitiallyVisible = !bFirstHidden;
                            Plan.Rooms.Add(R);

                            Plan.Triggers.Add(MakeTuple(ShopLocation, ShopShape));
                        }

                        // 계획 추가: 통로
                        PlanPassagesAlongRoute(RouteToA, Plan);
                        PlanPassagesAlongRoute(RouteToB, Plan);

                        bAnySpawnedThisAttempt = true;
                        return true;
                    }
                    return false;
                };

                bool bAllShopsPlaced = true;
                for (const auto& Pair : ShopPairs)
                {
                    if (!TryPlanShopBetweenLevels(Pair.Key, Pair.Value))
                    {
                        bAllShopsPlaced = false;
                        break;
                    }
                }

                if (!ShopPairs.IsEmpty() && !bAllShopsPlaced)
                {
                    // 상점 필수 → 이번 계획 폐기하고 전체 재시도
                    continue; // GlobalAttempt++
                }
            }
        }

        // // === DeadEnd(막다른 길) 계획 ===
        // {
        //     int32 DeadEndCnt = 3;
        //     int32 breakcnt = 0;
        //     while (DeadEndCnt > 0 && breakcnt < 1000)
        //     {
        //         TArray<FIntPoint> DirectionsLocal = { FIntPoint(0,1), FIntPoint(0,-1), FIntPoint(-1,0), FIntPoint(1,0) };

        //         const auto& RandomInfo = GetRandomElement(FinalMapInfos);
        //         const auto& BaseNode = RandomInfo.BaseNode;

        //         const auto& ModuleData = RandomInfo.LevelModuleData;
        //         const auto& LastLevelExit = GetRandomElement(ModuleData.LevelExit);
        //         const auto& LastNode = BaseNode + LastLevelExit.Key;
        //         auto LastDir = DirectionsLocal[(uint8)LastLevelExit.Value];
        //         LastDir = FIntPoint(-LastDir.X, -LastDir.Y);

        //         FLevelModuleData CurrentLevelData = GetRandomElement(LevelModuleData);
        //         FIntPoint Shape = CurrentLevelData.LevelShape;

        //         FIntPoint CurrentNode;
        //         TryGenerateNextRoomCandidate(LastNode, CurrentNode);

        //         TArray<FIntPoint> FinalRoute;
        //         TSet<FIntPoint> Occupied;
        //         TMap<uint8, TSet<FIntPoint>> Maps = RandomInfo.Maps;
        //         auto LevelNumber = RandomInfo.LevelNumber;

        //         if (!CanPlaceRoom(CurrentNode, Shape, Occupied))
        //         {
        //             breakcnt++;
        //             continue;
        //         }

        //         FinalRoute = FindPathBetweenNodes(CurrentLevelData, CurrentNode, LastDir, DirectionsLocal, LevelNumber, Occupied);
        //         if (FinalRoute.IsEmpty())
        //         {
        //             breakcnt++;
        //             continue;
        //         }

        //         for (const FIntPoint& P : Occupied) Used.Add(P);
        //         for (const FIntPoint& P : FinalRoute) Used.Add(P);

        //         // 계획 추가: 막다른길 방 + 통로
        //         {
        //             FVector LevelLocation = CalculateLevelLocation(CurrentNode, Shape);

        //             FPlannedSubLevel R;
        //             R.LevelPath = CurrentLevelData.LevelWorld.ToSoftObjectPath().ToString();
        //             R.Location = LevelLocation;
        //             R.Rotation = FRotator::ZeroRotator;
        //             R.bInitiallyVisible = !bFirstHidden;
        //             Plan.Rooms.Add(R);

        //             Plan.Triggers.Add(MakeTuple(LevelLocation, Shape));

        //             PlanPassagesAlongRoute(FinalRoute, Plan);
        //         }

        //         bAnySpawnedThisAttempt = true;
        //         breakcnt = 0;
        //         DeadEndCnt--;
        //     }
        // }

        // 최종 검증: 이번 시도에서 뭔가 만들었나?
        if (!bAnySpawnedThisAttempt)
        {
            continue; // GlobalAttempt++
        }

        // [FIX] 메인 경로 목표 미달 시에는 커밋하지 않고 재시도
        if (!bSuccessful || CurrentMainLevels < TargetMainLevels)
        {
            continue; // GlobalAttempt++
        }

        // 성공 → 벤치마크 데이터 저장
        const double SyncDuration = (FPlatformTime::Seconds() - SyncStartTime) * 1000.0;
        LastSyncBenchmark.TotalAsyncTime = SyncDuration; // 동기는 전부 메인 스레드
        LastSyncBenchmark.ComputationTime = SyncDuration;
        LastSyncBenchmark.ParallelForTime = 0.0f; // 병렬 처리 없음
        LastSyncBenchmark.ThreadCount = 1;
        LastSyncBenchmark.TotalAttempts = GlobalAttempt + 1;
        LastSyncBenchmark.SuccessfulThreads = 1;
        LastSyncBenchmark.GeneratedMainLevels = CurrentMainLevels;
        LastSyncBenchmark.TotalRooms = Plan.Rooms.Num();

        UE_LOG(LogTemp, Warning, TEXT("[SYNC Benchmark] Total Time: %.2f ms"), SyncDuration);
        UE_LOG(LogTemp, Display, TEXT("[SYNC Benchmark] Attempts: %d"), GlobalAttempt + 1);
        UE_LOG(LogTemp, Display, TEXT("[SYNC Benchmark] Generated Levels: %d / %d"), CurrentMainLevels, TargetMainLevels);

        // 성공 → 커밋!
        ApplyGenerationPlan(Plan);
        break; // 성공 탈출
    }
}

void AAPGameModeBattleStage::TryGenerateNextRoomCandidate(const FIntPoint& LastNode, FIntPoint& OutCurrentNode)
{
    // 1. 방향 중 하나를 랜덤 선택 
    OutCurrentNode = LastNode + FIntPoint(
        FMath::RandRange(-MaxMapDist, MaxMapDist),
        FMath::RandRange(-MaxMapDist, MaxMapDist)
    );

    // 2. 이전 노드보다 앞이면 무효
    if (OutCurrentNode.X + OutCurrentNode.Y <= LastNode.X + LastNode.Y)
    {
        TryGenerateNextRoomCandidate(LastNode, OutCurrentNode);
    }
}

// [UPDATED] Thread-safe 버전: Used와 BossOffset을 매개변수로 받음
bool AAPGameModeBattleStage::CanPlaceRoom(const FIntPoint& CurrentNode, const FIntPoint& Shape,
                                           const TSet<FIntPoint>& InUsed, TSet<FIntPoint>& OutOccupied,
                                           const FIntPoint& InBossOffset)
{
    bool bCanPlace = true;
    for (int32 x = 0; x < Shape.X; x++)
    {
        for (int32 y = 0; y < Shape.Y; y++)
        {
            FIntPoint Offset = CurrentNode + FIntPoint(x, y);
            if (Offset.X < 0 || Offset.X > InBossOffset.X || Offset.Y < 0 || Offset.Y > InBossOffset.Y)
            {
                bCanPlace = false;
                break;
            }
            if (InUsed.Contains(Offset))
            {
                bCanPlace = false;
                break;
            }
            OutOccupied.Add(Offset);
        }
        if (!bCanPlace) break;
    }
    return bCanPlace;
}

// [UPDATED] Thread-safe 버전: static 함수로 변경
FVector AAPGameModeBattleStage::CalculateLevelLocation(const FIntPoint& CurrentNode, const FIntPoint& Shape,
                                                        float RoomSpacingValue)
{
    FVector Location = FVector(CurrentNode.X * RoomSpacingValue, -CurrentNode.Y * RoomSpacingValue, 0.0f);
    Location.Y -= Shape.Y * RoomSpacingValue;
    return Location;
}

// [UPDATED] Thread-safe 버전: Used, TileNumber를 매개변수로 받음
TArray<FIntPoint> AAPGameModeBattleStage::FindPathBetweenNodes(const FLevelModuleData& CurrentLevelModuleData,
    const FIntPoint& CurrentNode, const FIntPoint& LastDir, TArray<FIntPoint> Directions,
    int32 CurrentNodeNumber, const TSet<FIntPoint>& Occupied,
    const TSet<FIntPoint>& InUsed, const TMap<FIntPoint, int32>& InTileNumber)
{
    TQueue<TPair<FIntPoint, TArray<FIntPoint>>> PathQueue;
    TSet<FIntPoint> Visited = InUsed;
    
    auto Entrance = GetRandomElement(CurrentLevelModuleData.LevelEntrance);
    FIntPoint Start = CurrentNode + Entrance.Key;
    FIntPoint FirstDir = Directions[(uint8)Entrance.Value];
    
    PathQueue.Enqueue(MakeTuple(Start + FirstDir, TArray<FIntPoint>({ Start, Start + FirstDir })));

    Visited.Add(Start);
    
    while (!PathQueue.IsEmpty())
    {
        TPair<FIntPoint, TArray<FIntPoint>> Cur;
        PathQueue.Dequeue(Cur);

        auto CurNode = Cur.Key;
        auto CurPath = Cur.Value;

        if (Occupied.Contains(CurNode)) continue;
        if (Visited.Contains(CurNode)) continue;  
        if (CurPath.Num() > MaxPathDist) continue;  

        Visited.Add(CurNode);

        // 랜덤 방향 섞기
        for (int32 i = 0; i < Directions.Num(); ++i)
        {
            int32 SwapIndex = FMath::RandRange(i, Directions.Num() - 1);
            if (i != SwapIndex)
            {
                Directions.Swap(i, SwapIndex);
            }
        }

        for (const FIntPoint& Dir : Directions)
        {
            FIntPoint Next = CurNode + Dir;
            TArray<FIntPoint> Paths = CurPath;

            Paths.Add(Next);
            PathQueue.Enqueue(MakeTuple(Next, Paths));

            if (InTileNumber.Contains(Next))
            {
                if( InTileNumber[Next] == CurrentNodeNumber && 
                    Dir == LastDir)
                {
                    return Paths; // 최종 경로 반환
                }
            }
        }
    }

    return TArray<FIntPoint>(); // 경로 없음
}

// void AArcanePunkPlayerController::StartLoading()
// {
//     if(MyCharacter.IsValid()) MyCharacter->DisableInput(this); 

//     auto UISubsystem = Cast<UAPUserWidgetSubsystem>(GetWorld()->GetGameInstance()->GetSubsystemBase(UAPUserWidgetSubsystem::StaticClass()));
    
//     if(UISubsystem) 
//     {
//         UISubsystem->CreateLoadingUI();
//     }
// }
// [CHANGED] 계획을 실제로 적용(스폰)하는 커밋 함수
// 이유: 성공이 확정됐을 때 한 번에 적용하기 위함
void AAPGameModeBattleStage::ApplyGenerationPlan(const FGenerationPlan& Plan)
{
    // 저장용 핸들
    PendingStreamLevels.Empty();
    LevelsLoadedCount = 0;
    TotalStreamLevels = Plan.Rooms.Num() + Plan.Passages.Num();
    UE_LOG(LogTemp, Display, TEXT("TotalStreamLevels : %d"), TotalStreamLevels);

    // 로딩 UI 시작
    UAPUserWidgetSubsystem::CreateLoadingUI(this);
    // UYourGameUIManager::ShowLoadingScreen();  // 프로젝트에 맞게 수정

    // -------- Rooms --------   
    for (const auto& R : Plan.Rooms)
    {
        bool bOutSuccess = false;
        ULevelStreamingDynamic* NewLevel = ULevelStreamingDynamic::LoadLevelInstance(
            GetWorld(), R.LevelPath, R.Location, R.Rotation, bOutSuccess
        );

        if (NewLevel)
        {
            NewLevel->SetShouldBeVisible(R.bInitiallyVisible);

            // 준비 완료 콜백 바인딩
            NewLevel->OnLevelShown.AddDynamic(this, &AAPGameModeBattleStage::OnSingleLevelLoaded);
            PendingStreamLevels.Add(NewLevel);
        }
    }

    // -------- Passages --------
    for (const auto& P : Plan.Passages)
    {
        bool bOutSuccess = false;
        ULevelStreamingDynamic* NewLevel = ULevelStreamingDynamic::LoadLevelInstance(
            GetWorld(), P.LevelPath, P.Location, P.Rotation, bOutSuccess
        );

        if (NewLevel)
        {
            NewLevel->SetShouldBeVisible(P.bInitiallyVisible);
            NewLevel->OnLevelShown.AddDynamic(this, &AAPGameModeBattleStage::OnSingleLevelLoaded);
            PendingStreamLevels.Add(NewLevel);
        }
    }

    // -------- Triggers --------
    for (const auto& T : Plan.Triggers)
    {
        const FVector LevelLocation = T.Get<0>();
        const FIntPoint Shape = T.Get<1>();

        FVector VolumeExtent = (FVector(Shape.X, Shape.Y, 1.0f) * RoomSpacing) / 25.f;
        AAPLevelStreamTrigger* Volume = GetWorld()->SpawnActor<AAPLevelStreamTrigger>(
            LevelStreamTriggerClass, LevelLocation, FRotator::ZeroRotator
        );
        if (Volume)
        {
            Volume->SetActorScale3D(VolumeExtent);
        }
    }
}

void AAPGameModeBattleStage::OnSingleLevelLoaded()
{
    LevelsLoadedCount++;

    // 진행률 UI 갱신
    const float Progress = (float)LevelsLoadedCount / (float)TotalStreamLevels;
    UAPUserWidgetSubsystem::UpdateLoadingPercent(this, Progress);

    UE_LOG(LogTemp, Display, TEXT("Stage Progress %f"), Progress);
    UE_LOG(LogTemp, Display, TEXT("LevelsLoadedCount : %d"), LevelsLoadedCount);

    // 전부 로드 완료 시
    if (LevelsLoadedCount >= TotalStreamLevels)
    {
        UAPUserWidgetSubsystem::RemoveLoadingUI(this);

        // 벤치마크 출력 (포트폴리오용)
        if (LevelGenerationConfig.bEnableBenchmarkLogging)
        {
            PrintBenchmark();

            // 양쪽 벤치마크가 모두 있으면 비교 출력
            if (LastSyncBenchmark.TotalRooms > 0 && LastAsyncBenchmark.TotalRooms > 0)
            {
                PrintBenchmarkComparison();
            }
        }
    }
}

// [CHANGED] 통로를 즉시 로드하는 대신 "계획"에 추가
void AAPGameModeBattleStage::PlanPassagesAlongRoute(const TArray<FIntPoint>& FinalRoute, FGenerationPlan& OutPlan)
{
    for (int32 i = 0; i < FinalRoute.Num(); i++)
    {
        FVector PassageLocation = FVector(FinalRoute[i].X * RoomSpacing, -FinalRoute[i].Y * RoomSpacing, 0.0f);
        FRotator PassageRotation = FRotator::ZeroRotator;
        TSoftObjectPtr<UWorld> PassageAsset;

        if(i == 0 && i+1 < FinalRoute.Num())
        {
            PassageAsset = PassageModule_Short;

            if(FinalRoute[i+1].X == FinalRoute[i].X)
            {
                PassageLocation.X += (RoomSpacing - PassageSpacing) * 0.5f;
                if(FinalRoute[i+1].Y - FinalRoute[i].Y == 1) { PassageLocation.Y -= RoomSpacing + PassageSpacing*0.5f; }
                else { PassageLocation.Y -= PassageSpacing*0.5f; }
            }
            else
            {
                PassageLocation.Y -= (RoomSpacing + PassageSpacing) * 0.5f;
                PassageRotation.Yaw = 90.0f;
                if(FinalRoute[i+1].X - FinalRoute[i].X == 1) { PassageLocation.X += RoomSpacing - PassageSpacing*0.5f; }
                else { PassageLocation.X -= PassageSpacing*0.5f; }
            }
        }

        if(i >= 1 && i+1 < FinalRoute.Num())
        {
            if(FinalRoute[i-1].X == FinalRoute[i+1].X)
            {
                PassageAsset = PassageModule_Long;

                PassageLocation.X += (RoomSpacing - PassageSpacing) * 0.5f;
                if(FinalRoute[i].Y - FinalRoute[i-1].Y == 1) { PassageLocation.Y -= RoomSpacing * 0.5f + PassageSpacing; }
                else { PassageLocation.Y -= RoomSpacing*0.5f; }
            }
            else if(FinalRoute[i-1].Y == FinalRoute[i+1].Y)
            {
                PassageAsset = PassageModule_Long;

                PassageLocation.Y -= (RoomSpacing + PassageSpacing) * 0.5f;
                PassageRotation.Yaw = 90.0f;

                if(FinalRoute[i].X - FinalRoute[i-1].X == 1) { PassageLocation.X += (RoomSpacing) * 0.5f; }
                else { PassageLocation.X += (RoomSpacing)*0.5f - PassageSpacing; }
            }
            else
            {
                auto Dir = FinalRoute[i+1] + FinalRoute[i-1] - FinalRoute[i] - FinalRoute[i] ;
                auto CornerSpace = RoomSpacing * 0.25f;
                PassageAsset = PassageModule_Corner;

                if(Dir == FIntPoint(-1,-1)) { PassageLocation.X += CornerSpace; PassageLocation.Y -= PassageSpacing + CornerSpace; }
                if(Dir == FIntPoint(1,1))   { PassageLocation.X += RoomSpacing - PassageSpacing - CornerSpace; PassageLocation.Y -= RoomSpacing - CornerSpace; PassageRotation.Yaw = 180.0f; }
                if(Dir == FIntPoint(1,-1))  { PassageLocation.X += RoomSpacing - PassageSpacing - CornerSpace; PassageLocation.Y -= PassageSpacing + CornerSpace; PassageRotation.Yaw = -90.0f; }
                if(Dir == FIntPoint(-1,1))  { PassageLocation.X += CornerSpace; PassageLocation.Y -= RoomSpacing - CornerSpace; PassageRotation.Yaw = 90.0f; }
            }
        }

        // 경로에서 어떤 노드에서든 Asset이 세팅되지 않을 수 있음 → 스킵
        if (!PassageAsset.IsNull())
        {
            FPlannedSubLevel P;
            P.LevelPath = PassageAsset.ToSoftObjectPath().ToString();
            P.Location = PassageLocation;
            P.Rotation = PassageRotation;
            P.bInitiallyVisible = !bFirstHidden;
            OutPlan.Passages.Add(P);
        }
    }
}

// ============================================================================
// [NEW] 멀티스레드 레벨 생성 구현
// ============================================================================

void AAPGameModeBattleStage::CreateLevelAsync()
{
    // Phase 1: Async Task로 메인 스레드 블로킹 방지
    UAPUserWidgetSubsystem::CreateLoadingUI(this);

    if (LevelGenerationConfig.bEnableBenchmarkLogging)
    {
        UE_LOG(LogTemp, Display, TEXT("===== [Async] Level Generation Started ====="));
        UE_LOG(LogTemp, Display, TEXT("[Config] Multithreading: %s"),
               LevelGenerationConfig.bUseMultithreading ? TEXT("Enabled") : TEXT("Disabled"));
        UE_LOG(LogTemp, Display, TEXT("[Config] ParallelFor: %s"),
               LevelGenerationConfig.bUseParallelFor ? TEXT("Enabled") : TEXT("Disabled"));
        UE_LOG(LogTemp, Display, TEXT("[Config] Thread Count: %d"),
               LevelGenerationConfig.GetEffectiveThreadCount());
    }

    const double AsyncStartTime = FPlatformTime::Seconds();

    // 백그라운드 스레드로 전달할 데이터 복사 (멤버 변수는 게임 스레드에서만 안전)
    const TArray<FLevelModuleData> LevelModuleDataCopy = LevelModuleData;
    const FLevelModuleData StartLevelCopy = StartLevelModuleData;
    const FLevelModuleData ShopLevelCopy = ShopLevelModuleData;
    const int32 TargetLevelsCopy = TargetMainLevels;
    const int32 MaxDistCopy = MaxMapDist;
    const int32 MaxPathDistCopy = MaxPathDist;
    const int32 TotalMaxDistCopy = TotalMaxLevelDist;
    const FIntPoint BossOffsetCopy = BossLevelOffset;
    const float RoomSpacingCopy = RoomSpacing;
    const bool bFirstHiddenCopy = bFirstHidden;

    // 백그라운드 스레드에서 레벨 생성 계획 수립
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, AsyncStartTime,
              LevelModuleDataCopy, StartLevelCopy, ShopLevelCopy, TargetLevelsCopy,
              MaxDistCopy, MaxPathDistCopy, TotalMaxDistCopy, BossOffsetCopy,
              RoomSpacingCopy, bFirstHiddenCopy]()
    {
        const double ComputeStartTime = FPlatformTime::Seconds();

        // Phase 2: ParallelFor로 여러 시도를 병렬로 수행
        FGenerationPlan Plan = ComputeLevelGeneration(LevelModuleDataCopy, StartLevelCopy, ShopLevelCopy,
                                                      TargetLevelsCopy, MaxDistCopy, MaxPathDistCopy,
                                                      TotalMaxDistCopy, BossOffsetCopy,
                                                      RoomSpacingCopy, bFirstHiddenCopy);

        const double ComputeDuration = (FPlatformTime::Seconds() - ComputeStartTime) * 1000.0;

        if (LevelGenerationConfig.bEnableBenchmarkLogging)
        {
            UE_LOG(LogTemp, Warning, TEXT("[Benchmark] Background Computation Time: %.2f ms"), ComputeDuration);
        }

        // 게임 스레드로 돌아와서 실제 레벨 로드 (UObject 조작 필요)
        AsyncTask(ENamedThreads::GameThread, [this, Plan, AsyncStartTime, ComputeDuration]()
        {
            const double TotalDuration = (FPlatformTime::Seconds() - AsyncStartTime) * 1000.0;

            // 벤치마크 데이터 저장
            LastAsyncBenchmark.TotalAsyncTime = TotalDuration;
            LastAsyncBenchmark.ComputationTime = ComputeDuration;
            LastAsyncBenchmark.ThreadCount = LevelGenerationConfig.GetEffectiveThreadCount();
            LastAsyncBenchmark.GeneratedMainLevels = TargetMainLevels;
            LastAsyncBenchmark.TotalRooms = Plan.Rooms.Num();

            if (LevelGenerationConfig.bEnableBenchmarkLogging)
            {
                UE_LOG(LogTemp, Display, TEXT("[Benchmark] Total Async Task Time: %.2f ms"), TotalDuration);
                UE_LOG(LogTemp, Display, TEXT("[Benchmark] - Computation: %.2f ms"), ComputeDuration);
                UE_LOG(LogTemp, Display, TEXT("[Benchmark] - Thread Switching: %.2f ms"), TotalDuration - ComputeDuration);
                UE_LOG(LogTemp, Display, TEXT("===== [Async] Applying Generation Plan ====="));
            }

            ApplyGenerationPlan(Plan);
        });
    });
}

void AAPGameModeBattleStage::PrintBenchmark() const
{
    if (bUseAsyncLevelGeneration)
    {
        UE_LOG(LogTemp, Display, TEXT("===== ASYNC Benchmark ====="));
        UE_LOG(LogTemp, Display, TEXT("%s"), *LastAsyncBenchmark.ToString());
        UE_LOG(LogTemp, Display, TEXT("CSV: %s"), *FProceduralLevelGenerationUtils::BenchmarkToCSV(LastAsyncBenchmark));
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("===== SYNC Benchmark ====="));
        UE_LOG(LogTemp, Display, TEXT("%s"), *LastSyncBenchmark.ToString());
        UE_LOG(LogTemp, Display, TEXT("CSV: %s"), *FProceduralLevelGenerationUtils::BenchmarkToCSV(LastSyncBenchmark));
    }
}

void AAPGameModeBattleStage::PrintBenchmarkComparison() const
{
    UE_LOG(LogTemp, Warning, TEXT("========================================"));
    UE_LOG(LogTemp, Warning, TEXT("  SYNC vs ASYNC Performance Comparison"));
    UE_LOG(LogTemp, Warning, TEXT("========================================"));

    const float SyncTime = LastSyncBenchmark.ComputationTime;
    const float AsyncTime = LastAsyncBenchmark.ComputationTime;
    const float Speedup = SyncTime > 0.0f ? (SyncTime / FMath::Max(0.001f, AsyncTime)) : 0.0f;
    const float TimeReduction = SyncTime - AsyncTime;
    const float ReductionPercent = SyncTime > 0.0f ? (TimeReduction / SyncTime * 100.0f) : 0.0f;

    UE_LOG(LogTemp, Display, TEXT(""));
    UE_LOG(LogTemp, Display, TEXT("[SYNC] Total Time: %.2f ms"), SyncTime);
    UE_LOG(LogTemp, Display, TEXT("[SYNC] Thread Count: %d"), LastSyncBenchmark.ThreadCount);
    UE_LOG(LogTemp, Display, TEXT("[SYNC] Attempts: %d"), LastSyncBenchmark.TotalAttempts);
    UE_LOG(LogTemp, Display, TEXT(""));
    UE_LOG(LogTemp, Display, TEXT("[ASYNC] Total Time: %.2f ms"), AsyncTime);
    UE_LOG(LogTemp, Display, TEXT("[ASYNC] Thread Count: %d"), LastAsyncBenchmark.ThreadCount);
    UE_LOG(LogTemp, Display, TEXT("[ASYNC] ParallelFor Time: %.2f ms"), LastAsyncBenchmark.ParallelForTime);
    UE_LOG(LogTemp, Display, TEXT("[ASYNC] Successful Threads: %d / %d"),
           LastAsyncBenchmark.SuccessfulThreads, LastAsyncBenchmark.ThreadCount);
    UE_LOG(LogTemp, Display, TEXT(""));
    UE_LOG(LogTemp, Warning, TEXT("[RESULT] Speedup: %.2fx faster"), Speedup);
    UE_LOG(LogTemp, Warning, TEXT("[RESULT] Time Saved: %.2f ms (%.1f%% reduction)"),
           TimeReduction, ReductionPercent);
    UE_LOG(LogTemp, Display, TEXT(""));

    // 포트폴리오용 CSV
    UE_LOG(LogTemp, Display, TEXT("CSV Header: Method,Time,ThreadCount,Attempts,Speedup"));
    UE_LOG(LogTemp, Display, TEXT("CSV Sync: SYNC,%.2f,%d,%d,1.00"),
           SyncTime, LastSyncBenchmark.ThreadCount, LastSyncBenchmark.TotalAttempts);
    UE_LOG(LogTemp, Display, TEXT("CSV Async: ASYNC,%.2f,%d,%d,%.2f"),
           AsyncTime, LastAsyncBenchmark.ThreadCount, LastAsyncBenchmark.TotalAttempts, Speedup);

    UE_LOG(LogTemp, Warning, TEXT("========================================"));
}

FGenerationPlan AAPGameModeBattleStage::ComputeLevelGeneration(
    const TArray<FLevelModuleData>& InLevelModules,
    const FLevelModuleData& InStartLevel,
    const FLevelModuleData& InShopLevel,
    int32 InTargetLevels, int32 InMaxDist, int32 InMaxPathDist,
    int32 InTotalMaxDist, FIntPoint InBossOffset,
    float InRoomSpacing, bool bInFirstHidden)
{
    // Phase 2: ParallelFor로 병렬 시도
    const int32 NumThreads = LevelGenerationConfig.GetEffectiveThreadCount();
    const int32 AttemptsPerThread = LevelGenerationConfig.AttemptsPerThread;

    if (LevelGenerationConfig.bEnableBenchmarkLogging)
    {
        UE_LOG(LogTemp, Display, TEXT("[ParallelFor] Using %d threads, %d attempts per thread"),
               NumThreads, AttemptsPerThread);
    }

    TArray<FGenerationPlan> Candidates;
    TArray<int32> MainLevelCounts;
    Candidates.SetNum(NumThreads);
    MainLevelCounts.SetNum(NumThreads);

    const double ParallelStartTime = FPlatformTime::Seconds();

    // 각 스레드가 독립적으로 시도
    ParallelFor(NumThreads, [&](int32 ThreadIdx)
    {
        const double ThreadStartTime = FPlatformTime::Seconds();
        int32 BestMainLevels = 0;

        // 각 스레드가 여러 번 시도
        for (int32 LocalAttempt = 0; LocalAttempt < AttemptsPerThread; ++LocalAttempt)
        {
            int32 GeneratedMainLevels = 0;
            const int32 GlobalAttemptIndex = ThreadIdx * AttemptsPerThread + LocalAttempt;

            FGenerationPlan LocalPlan = TryGenerateLevelPlan(GlobalAttemptIndex, GeneratedMainLevels,
                                                            InLevelModules, InStartLevel, InShopLevel,
                                                            InTargetLevels, InMaxDist, InMaxPathDist,
                                                            InTotalMaxDist, InBossOffset,
                                                            InRoomSpacing, bInFirstHidden);

            // 목표 달성 시 저장하고 중단
            if (GeneratedMainLevels >= InTargetLevels)
            {
                Candidates[ThreadIdx] = LocalPlan;
                MainLevelCounts[ThreadIdx] = GeneratedMainLevels;

                if (LevelGenerationConfig.bEnableVerboseLogging)
                {
                    const double ThreadDuration = (FPlatformTime::Seconds() - ThreadStartTime) * 1000.0;
                    UE_LOG(LogTemp, Display, TEXT("[Thread %d] Success after %d attempts (%.2f ms)"),
                           ThreadIdx, LocalAttempt + 1, ThreadDuration);
                }
                break;
            }

            // 부분 성공이라도 가장 좋은 것 보관
            if (GeneratedMainLevels > BestMainLevels)
            {
                BestMainLevels = GeneratedMainLevels;
                Candidates[ThreadIdx] = LocalPlan;
                MainLevelCounts[ThreadIdx] = GeneratedMainLevels;
            }
        }
    }, EParallelForFlags::BackgroundPriority);

    const double ParallelDuration = (FPlatformTime::Seconds() - ParallelStartTime) * 1000.0;

    // 벤치마크 데이터 업데이트
    LastAsyncBenchmark.ParallelForTime = ParallelDuration;
    LastAsyncBenchmark.TotalAttempts = NumThreads * AttemptsPerThread;

    // 성공한 스레드 수 계산
    int32 SuccessCount = 0;
    for (int32 Count : MainLevelCounts)
    {
        if (Count >= InTargetLevels)
        {
            SuccessCount++;
        }
    }
    LastAsyncBenchmark.SuccessfulThreads = SuccessCount;

    if (LevelGenerationConfig.bEnableBenchmarkLogging)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Benchmark] ParallelFor Total Time: %.2f ms"), ParallelDuration);
        UE_LOG(LogTemp, Display, TEXT("[Benchmark] Successful Threads: %d / %d"), SuccessCount, NumThreads);
    }

    // 가장 좋은 결과 선택
    return SelectBestPlan(Candidates, MainLevelCounts);
}

