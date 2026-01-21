
#include "LevelStream/APLevelStreamingVolume.h"

#include "Kismet/GameplayStatics.h"
#include "GameInstance/APGameInstance.h"
#include "GameElements/Prop/APBackGroundAssetBase.h"

void AAPLevelStreamingVolume::PostLoad()
{
    Super::PostLoad();
                    
}

void AAPLevelStreamingVolume::BeginPlay()
{
    Super::BeginPlay();

    // Capture memory before loading
    FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
    MemoryBeforeLoad = MemoryStats.UsedPhysical / (1024.0f * 1024.0f);

    StreamingUsage = EStreamingVolumeUsage::SVB_VisibilityBlockingOnLoad;
    for(auto LevelName : StreamingLevelNames)
    {
        auto StreamingLevel = UGameplayStatics::GetStreamingLevel(this, LevelName);
        if(StreamingLevel)
        {
            StreamingLevels.Emplace(StreamingLevel);
        }
    }

    if(StreamingLevels.IsEmpty()) return;
    StreamingLevels.Top()->OnLevelLoaded.RemoveDynamic(this, &AAPLevelStreamingVolume::OnLevelLoadComplete);
    StreamingLevels.Top()->OnLevelLoaded.AddDynamic(this, &AAPLevelStreamingVolume::OnLevelLoadComplete);

    UE_LOG(LogTemp, Log, TEXT("[LevelStreamingVolume] BeginPlay - Levels to load: %d"), StreamingLevels.Num());
}

void AAPLevelStreamingVolume::OnLevelLoadComplete()
{
    if(StreamingLevels.IsEmpty()) return;

    bool bAllActorsReady = true;
    TotalActorsToCheck = 0;
    ActorsCompleted = 0;

    for(auto StreamingLevel : StreamingLevels)
    {
        ULevel* LoadedLevel = StreamingLevel->GetLoadedLevel();
        if (!LoadedLevel) {bAllActorsReady = false; break;}

        for (AActor* Actor : LoadedLevel->Actors)
        {
            auto SpawnActor = Cast<AAPBackGroundAssetBase>(Actor); if(!SpawnActor) continue;
            TotalActorsToCheck++;

            if (SpawnActor->IsSpawnCompleted())
            {
                ActorsCompleted++;
            }
            else
            {
                bAllActorsReady = false;
            }
        }

        if(!bAllActorsReady) break;
    }

    if (!bAllActorsReady)
    {
        // Log progress
        const float Progress = GetLoadingProgress();
        UE_LOG(LogTemp, Log, TEXT("[LevelStreamingVolume] Loading Progress: %.1f%% (%d / %d actors)"),
            Progress * 100.0f, ActorsCompleted, TotalActorsToCheck);

        GetWorld()->GetTimerManager().SetTimer(CheckActorsHandle, this, &AAPLevelStreamingVolume::OnLevelLoadComplete, 0.15f, false);
    }
    else
    {
        // All actors ready - log memory usage
        LogMemoryUsage();
        UE_LOG(LogTemp, Log, TEXT("[LevelStreamingVolume] All actors ready - Loading complete!"));
    }
}

float AAPLevelStreamingVolume::GetLoadingProgress() const
{
    if (TotalActorsToCheck == 0) return 0.0f;
    return static_cast<float>(ActorsCompleted) / TotalActorsToCheck;
}

void AAPLevelStreamingVolume::LogMemoryUsage() const
{
    FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
    const float CurrentMemory = MemoryStats.UsedPhysical / (1024.0f * 1024.0f);
    const float MemoryDelta = CurrentMemory - MemoryBeforeLoad;

    UE_LOG(LogTemp, Log, TEXT("[LevelStreamingVolume] Memory Usage: %.2f MB (Delta: %+.2f MB)"),
        CurrentMemory, MemoryDelta);
}

