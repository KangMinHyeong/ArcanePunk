// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/APGameModeBase.h"
#include "Templates/Tuple.h"
#include "Async/ProceduralLevelGenerationConfig.h"
#include "APGameModeBattleStage.generated.h"

class UNiagaraSystem;
class USoundBase;
class ALevelInstance;
class AAPLevelStreamTrigger;
class ULevelStreamingDynamic;


#pragma region Enum_and_Struct  
UENUM(BlueprintType)
enum class ELevelDirection : uint8
{
    Up   	   = 0 UMETA(DisplayName = "상"),
    Down	   = 1 UMETA(DisplayName = "하"),
    Left	   = 2 UMETA(DisplayName = "좌"),
	Right 	   = 3 UMETA(DisplayName = "우"),
};

USTRUCT(BlueprintType)
struct FLevelModuleData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
    TSoftObjectPtr<UWorld> LevelWorld;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
    FIntPoint LevelShape;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
    TMap<FIntPoint, ELevelDirection> LevelEntrance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
    TMap<FIntPoint, ELevelDirection> LevelExit;
};

USTRUCT(BlueprintType)
struct FGeneratedLevelModuleInfo
{
    GENERATED_BODY()   

	UPROPERTY()
    FIntPoint BaseNode;
	UPROPERTY()
    FLevelModuleData LevelModuleData; 
    UPROPERTY()
    TArray<FIntPoint> Paths; 	

	// 길찾기 중복에 필요한 원소
	int32 TotalLevelDist = 1;
	int32 LevelNumber = 1;
	TSet<FIntPoint> Used;
	TMap<FIntPoint, int32> TileNumber;
	TMap<uint8, TSet<FIntPoint>> Maps;  
};

USTRUCT()
struct FPlannedSubLevel
{
    GENERATED_BODY()

    UPROPERTY() FString LevelPath;
    UPROPERTY() FVector Location;
    UPROPERTY() FRotator Rotation;
    UPROPERTY() bool bInitiallyVisible = true;
};

USTRUCT()
struct FGenerationPlan
{
    GENERATED_BODY()

    // 실제 로드할 "방" 서브레벨들
    UPROPERTY() TArray<FPlannedSubLevel> Rooms;
    // 실제 로드할 "통로" 서브레벨들
    UPROPERTY() TArray<FPlannedSubLevel> Passages;
    // 트리거 스폰에 필요한 데이터 (여기선 위치/스케일만 저장)
    TArray<TTuple<FVector /*LevelLocation*/, FIntPoint /*Shape*/>> Triggers;
};

#pragma endregion


#pragma once
// TArray
template<typename T>
FORCEINLINE const T GetRandomElement(const TArray<T>& Array)
{
    // check(Array.Num() > 0); // 배열이 비었으면 크래시
    return Array[FMath::RandHelper(Array.Num())];
}

// TMap
template<typename KeyType, typename ValueType>
FORCEINLINE TPair<KeyType, ValueType> GetRandomElement(const TMap<KeyType, ValueType>& Map)
{
    // check(Map.Num() > 0); // 맵이 비었으면 크래시
	TArray<KeyType> Keys;
    Map.GetKeys(Keys);

    const KeyType& RandomKey = Keys[FMath::RandHelper(Keys.Num())];
    const ValueType& RandomValue = Map.FindChecked(RandomKey);

    return TPair<KeyType, ValueType>(RandomKey, RandomValue);
}

UCLASS()
class ARCANEPUNK_API AAPGameModeBattleStage : public AAPGameModeBase
{
	GENERATED_BODY()

// Func
protected:
	virtual void StartPlay() override;

#pragma region Getter
public:
	FORCEINLINE UNiagaraSystem* GetSpawnEffect() const {return SpawnEffect;};
	FORCEINLINE USoundBase* GetSpawnSound() const {return SpawnSound;};
	FORCEINLINE float GetSpawnSoundVolume() const {return SpawnSoundVolume;};
#pragma endregion

#pragma region Async Level Generation - Public Interface
public:
	// 벤치마크 결과 조회 (Blueprint에서 사용)
	UFUNCTION(BlueprintCallable, Category = "Async Level Generation")
	FLevelGenerationBenchmark GetLastAsyncBenchmark() const { return LastAsyncBenchmark; }

	UFUNCTION(BlueprintCallable, Category = "Async Level Generation")
	FLevelGenerationBenchmark GetLastSyncBenchmark() const { return LastSyncBenchmark; }

	// 벤치마크 결과 출력
	UFUNCTION(BlueprintCallable, Category = "Async Level Generation")
	void PrintBenchmark() const;

	// 동기 vs 비동기 비교 출력 (포트폴리오용)
	UFUNCTION(BlueprintCallable, Category = "Async Level Generation")
	void PrintBenchmarkComparison() const;
#pragma endregion

#pragma region Checking Stage Clear 
public:
	void PlayerKilled();
	void OnEndedGame();
private:
	void CheckClearStage(AActor* SpawnVolume);
	void PortalSpawn(FName CurrentClearStage);
#pragma endregion


#pragma region Create Level // 절차적 레벨 생성
private:
	// [DEPRECATED] 기존 동기 방식 (벤치마크 비교용으로 유지)
	void CreateLevel();

	// [NEW] 비동기 레벨 생성 (멀티스레드)
	void CreateLevelAsync();

	// [NEW] Phase 2: ParallelFor로 병렬 생성 계획 수립
	FGenerationPlan ComputeLevelGeneration(const TArray<FLevelModuleData>& InLevelModules,
	                                       const FLevelModuleData& InStartLevel,
	                                       const FLevelModuleData& InShopLevel,
	                                       int32 InTargetLevels, int32 InMaxDist, int32 InMaxPathDist,
	                                       int32 InTotalMaxDist, FIntPoint InBossOffset,
	                                       float InRoomSpacing, bool bInFirstHidden);

	// [NEW] 단일 시도로 레벨 계획 생성 (Thread-safe)
	FGenerationPlan TryGenerateLevelPlan(int32 AttemptIndex, int32& OutMainLevelsGenerated,
	                                     const TArray<FLevelModuleData>& InLevelModules,
	                                     const FLevelModuleData& InStartLevel,
	                                     const FLevelModuleData& InShopLevel,
	                                     int32 InTargetLevels, int32 InMaxDist, int32 InMaxPathDist,
	                                     int32 InTotalMaxDist, FIntPoint InBossOffset,
	                                     float InRoomSpacing, bool bInFirstHidden);

	// [NEW] 여러 후보 중 최적 계획 선택
	FGenerationPlan SelectBestPlan(const TArray<FGenerationPlan>& Candidates, const TArray<int32>& MainLevelCounts);

	// 방 배치 및 유효성 검사 (Thread-safe하게 매개변수로 변경)
	void TryGenerateNextRoomCandidate(const FIntPoint& LastNode, FIntPoint& OutCurrentNode);
    bool CanPlaceRoom(const FIntPoint& CurrentNode, const FIntPoint& Shape, const TSet<FIntPoint>& InUsed, TSet<FIntPoint>& OutOccupied, const FIntPoint& InBossOffset);

    // 레벨 위치 및 경로 계산 (Static으로 변경)
    static FVector CalculateLevelLocation(const FIntPoint& CurrentNode, const FIntPoint& Shape, float RoomSpacingValue);
    TArray<FIntPoint> FindPathBetweenNodes(const FLevelModuleData& CurrentLevelModuleData, const FIntPoint& CurrentNode, const FIntPoint& LastDir, TArray<FIntPoint> Directions, int32 CurrentNodeNumber, const TSet<FIntPoint>& Occupied, const TSet<FIntPoint>& InUsed, const TMap<FIntPoint, int32>& InTileNumber);

	void ApplyGenerationPlan(const FGenerationPlan& Plan);
	void PlanPassagesAlongRoute(const TArray<FIntPoint>& FinalRoute, FGenerationPlan& OutPlan);

	UFUNCTION()
	void OnSingleLevelLoaded();
#pragma endregion

// Variable

#pragma region TimeHandle Setting
	FTimerHandle PortalSpawnTimerHandle;
#pragma endregion

#pragma region Spawn Setting
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* SpawnEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* SpawnSound;

	UPROPERTY(EditAnywhere)
	float SpawnSoundVolume = 2.5f;
#pragma endregion

#pragma region Level Setting
	
	UPROPERTY(EditAnywhere)
	TArray<FLevelModuleData> LevelModuleData;

	UPROPERTY(EditAnywhere)
	FLevelModuleData StartLevelModuleData;
	UPROPERTY(EditAnywhere)
	FLevelModuleData EndLevelModuleData;
	UPROPERTY(EditAnywhere)
	FLevelModuleData ShopLevelModuleData; // 헤더에 선언 필요!

	// Level 설정
	UPROPERTY(EditAnywhere, Category="Level Setting")
	int32 MaxMapDist = 2;
	UPROPERTY(EditAnywhere, Category="Level Setting")
	int32 MaxPathDist = 5; // 통로 당 최대 길이
	UPROPERTY(EditAnywhere, Category="Level Setting")
	int32 TotalMaxLevelDist = 100; // 총 최대 맵 길이 ()
	UPROPERTY(EditAnywhere, Category="Level Setting")
	int32 SupplyNodeCount = 2;

	// 보스방 위치
	UPROPERTY(EditAnywhere)
	FIntPoint BossLevelOffset;

	// 임시변수
	UPROPERTY(EditAnywhere)
	bool bFirstHidden = true;

	// 규격 단위
	UPROPERTY(EditAnywhere, Category="Standard")
	float RoomSpacing = 2000.0f;
	UPROPERTY(EditAnywhere, Category="Standard")
	float PassageSpacing = 485.0f;

	// 통로 모듈
	UPROPERTY(EditAnywhere, Category="Passage")
	TSoftObjectPtr<UWorld> PassageModule_Short;
	UPROPERTY(EditAnywhere, Category="Passage")
	TSoftObjectPtr<UWorld> PassageModule_Long;
	UPROPERTY(EditAnywhere, Category="Passage")
	TSoftObjectPtr<UWorld> PassageModule_Corner; // ㄱ 모양 

	UPROPERTY(EditAnywhere)
	TSubclassOf<AAPLevelStreamTrigger> LevelStreamTriggerClass;

	// 길찾기 용 멤버변수
	TSet<FIntPoint> Used;
	TMap<FIntPoint, int32> TileNumber;

	UPROPERTY(EditAnywhere)
	int32 TargetMainLevels = 10;   

	UPROPERTY()
	TArray<ULevelStreamingDynamic*> PendingStreamLevels;

	int32 TotalStreamLevels = 0;
	int32 LevelsLoadedCount = 0;

#pragma endregion

#pragma region Async Level Generation - Variables
public:
	// 비동기 멀티스레드 사용 여부 (비교 테스트용)
	UPROPERTY(EditAnywhere, Category = "Async Level Generation")
	bool bUseAsyncLevelGeneration = true;

	// 절차적 레벨 생성 설정
	UPROPERTY(EditAnywhere, Category = "Async Level Generation")
	FProceduralLevelGenerationConfig LevelGenerationConfig;

	// 마지막 비동기 벤치마크 결과 
	// Config로 PIE 세션 간 유지됨
	UPROPERTY(Config, BlueprintReadOnly, Category = "Async Level Generation")
	FLevelGenerationBenchmark LastAsyncBenchmark;

	// 마지막 동기 벤치마크 결과 (비교용)
	// Config로 PIE 세션 간 유지됨
	UPROPERTY(Config, BlueprintReadOnly, Category = "Async Level Generation")
	FLevelGenerationBenchmark LastSyncBenchmark;
#pragma endregion


};
