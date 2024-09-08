
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

}

void AAPLevelStreamingVolume::OnLevelLoadComplete()
{
    if(StreamingLevels.IsEmpty()) return;

    bool bAllActorsReady = true;
    for(auto StreamingLevel : StreamingLevels)
    {
        ULevel* LoadedLevel = StreamingLevel->GetLoadedLevel();
        if (!LoadedLevel) {bAllActorsReady = false; break;}    
        
        for (AActor* Actor : LoadedLevel->Actors)
        {  
            auto SpawnActor = Cast<AAPBackGroundAssetBase>(Actor); if(!SpawnActor) continue;
            if (!SpawnActor->IsSpawnCompleted()) {bAllActorsReady = false; break;}    

        }     

        if(!bAllActorsReady) break;
    }

    GetWorld()->GetTimerManager().ClearTimer(CheckActorsHandle);
    if (bAllActorsReady)
    {
        GetWorld()->GetTimerManager().SetTimer(CheckActorsHandle, this, &AAPLevelStreamingVolume::HideLoadingScreen, 0.45f, false);
    }
    else
    {
        GetWorld()->GetTimerManager().SetTimer(CheckActorsHandle, this, &AAPLevelStreamingVolume::OnLevelLoadComplete, 0.15f, false);
    }
}

void AAPLevelStreamingVolume::HideLoadingScreen()
{
    GetWorld()->GetTimerManager().ClearTimer(CheckActorsHandle);
    auto APGI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); 
    if(APGI) APGI->OnStartFadeIn.Broadcast();
}
