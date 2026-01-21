// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralLevelGenerationConfig.generated.h"

/**
 * 절차적 레벨 생성 벤치마크 결과
 * 멀티스레드 성능 측정 및 포트폴리오용 데이터
 */
USTRUCT(BlueprintType)
struct FLevelGenerationBenchmark
{
	GENERATED_BODY()

	// 전체 비동기 작업 시간 (ms)
	UPROPERTY(BlueprintReadOnly, Category = "Benchmark")
	float TotalAsyncTime = 0.0f;

	// 백그라운드 계산 시간 (ms)
	UPROPERTY(BlueprintReadOnly, Category = "Benchmark")
	float ComputationTime = 0.0f;

	// ParallelFor 실행 시간 (ms)
	UPROPERTY(BlueprintReadOnly, Category = "Benchmark")
	float ParallelForTime = 0.0f;

	// 사용된 스레드 수
	UPROPERTY(BlueprintReadOnly, Category = "Benchmark")
	int32 ThreadCount = 0;

	// 총 시도 횟수
	UPROPERTY(BlueprintReadOnly, Category = "Benchmark")
	int32 TotalAttempts = 0;

	// 성공한 스레드 수
	UPROPERTY(BlueprintReadOnly, Category = "Benchmark")
	int32 SuccessfulThreads = 0;

	// 생성된 메인 레벨 수
	UPROPERTY(BlueprintReadOnly, Category = "Benchmark")
	int32 GeneratedMainLevels = 0;

	// 생성된 총 방 개수 (상점 포함)
	UPROPERTY(BlueprintReadOnly, Category = "Benchmark")
	int32 TotalRooms = 0;

	FLevelGenerationBenchmark()
		: TotalAsyncTime(0.0f)
		, ComputationTime(0.0f)
		, ParallelForTime(0.0f)
		, ThreadCount(0)
		, TotalAttempts(0)
		, SuccessfulThreads(0)
		, GeneratedMainLevels(0)
		, TotalRooms(0)
	{}

	// 성능 향상 계산 (동기 대비)
	float GetSpeedupFactor(float SyncTime) const
	{
		return SyncTime > 0.0f ? (SyncTime / FMath::Max(0.001f, ComputationTime)) : 0.0f;
	}

	// 벤치마크 요약 문자열
	FString ToString() const
	{
		return FString::Printf(
			TEXT("===== Level Generation Benchmark =====\n")
			TEXT("Total Async Time: %.2f ms\n")
			TEXT("Computation Time: %.2f ms\n")
			TEXT("ParallelFor Time: %.2f ms\n")
			TEXT("Threads Used: %d\n")
			TEXT("Total Attempts: %d\n")
			TEXT("Successful Threads: %d\n")
			TEXT("Generated Main Levels: %d\n")
			TEXT("Total Rooms: %d\n")
			TEXT("====================================="),
			TotalAsyncTime,
			ComputationTime,
			ParallelForTime,
			ThreadCount,
			TotalAttempts,
			SuccessfulThreads,
			GeneratedMainLevels,
			TotalRooms
		);
	}
};

/**
 * 절차적 레벨 생성 설정
 * 멀티스레드 동작 제어 및 최적화 파라미터
 */
USTRUCT(BlueprintType)
struct FProceduralLevelGenerationConfig
{
	GENERATED_BODY()

	// 멀티스레드 사용 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threading")
	bool bUseMultithreading = true;

	// 병렬 처리 사용 여부 (ParallelFor)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threading")
	bool bUseParallelFor = true;

	// 커스텀 스레드 수 (0 = 자동, CPU 코어 수 사용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threading", meta = (ClampMin = "0", ClampMax = "32"))
	int32 CustomThreadCount = 0;

	// 스레드당 최대 시도 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation", meta = (ClampMin = "10", ClampMax = "1000"))
	int32 AttemptsPerThread = 125;

	// 벤치마크 로깅 활성화
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bEnableBenchmarkLogging = true;

	// 상세 로깅 활성화 (각 스레드 진행 상황)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bEnableVerboseLogging = false;

	FProceduralLevelGenerationConfig()
		: bUseMultithreading(true)
		, bUseParallelFor(true)
		, CustomThreadCount(0)
		, AttemptsPerThread(125)
		, bEnableBenchmarkLogging(true)
		, bEnableVerboseLogging(false)
	{}

	// 실제 사용할 스레드 수 계산
	int32 GetEffectiveThreadCount() const
	{
		if (CustomThreadCount > 0)
		{
			return CustomThreadCount;
		}
		return FPlatformMisc::NumberOfCoresIncludingHyperthreads();
	}

	// 설정 유효성 검사
	bool IsValid() const
	{
		return AttemptsPerThread > 0 && GetEffectiveThreadCount() > 0;
	}
};

/**
 * 절차적 레벨 생성 유틸리티 함수
 */
class ARCANEPUNK_API FProceduralLevelGenerationUtils
{
public:
	// 멀티스레드 성능 예측
	static float EstimateSpeedup(int32 ThreadCount, float SyncTime)
	{
		// Amdahl's Law 적용 (병렬화 비율 95% 가정)
		const float ParallelFraction = 0.95f;
		const float SerialFraction = 1.0f - ParallelFraction;

		float Speedup = 1.0f / (SerialFraction + (ParallelFraction / ThreadCount));
		return Speedup;
	}

	// 권장 스레드 수 계산
	static int32 GetRecommendedThreadCount()
	{
		int32 CoreCount = FPlatformMisc::NumberOfCoresIncludingHyperthreads();

		// 게임 스레드를 위해 1개 코어 예약
		return FMath::Max(1, CoreCount - 1);
	}

	// 벤치마크 결과를 CSV 형식으로 변환 (포트폴리오용)
	static FString BenchmarkToCSV(const FLevelGenerationBenchmark& Benchmark)
	{
		return FString::Printf(
			TEXT("%f,%f,%f,%d,%d,%d,%d,%d"),
			Benchmark.TotalAsyncTime,
			Benchmark.ComputationTime,
			Benchmark.ParallelForTime,
			Benchmark.ThreadCount,
			Benchmark.TotalAttempts,
			Benchmark.SuccessfulThreads,
			Benchmark.GeneratedMainLevels,
			Benchmark.TotalRooms
		);
	}

	// CSV 헤더
	static FString GetCSVHeader()
	{
		return TEXT("TotalAsyncTime,ComputationTime,ParallelForTime,ThreadCount,TotalAttempts,SuccessfulThreads,GeneratedMainLevels,TotalRooms");
	}
};
