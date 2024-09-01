
#include "LevelStream/APLevelStreamingVolume.h"

#include "Kismet/GameplayStatics.h"
#include "GameInstance/APGameInstance.h"

void AAPLevelStreamingVolume::PostLoad()
{
    Super::PostLoad();
    
    for(auto LevelName : StreamingLevelNames)
    {
        StreamingLevel = UGameplayStatics::GetStreamingLevel(this, LevelName);
        if(StreamingLevel.IsValid())
        {
            StreamingLevel->OnLevelLoaded.AddDynamic(this, &AAPLevelStreamingVolume::OnLevelLoaded);
        }
    }
}

void AAPLevelStreamingVolume::OnLevelLoaded()
{
    if(!StreamingLevel.IsValid()) return;
    ULevel* LoadedLevel = StreamingLevel->GetLoadedLevel();

    if (LoadedLevel)
    {
        bool bAllActorsReady = true;
        for (AActor* Actor : LoadedLevel->Actors)
        {
            if (Actor && !Actor->HasActorBegunPlay())
            {
                bAllActorsReady = false;
                break;
            }
        }
        
        if (bAllActorsReady)
        {
            // 모든 오브젝트가 스폰되었습니다.
            HideLoadingScreen();
        }
        else
        {
            // 모든 오브젝트가 스폰될 때까지 대기하는 로직 추가
            GetWorld()->GetTimerManager().SetTimer(CheckActorsHandle, this, &AAPLevelStreamingVolume::CheckAllActorsLoaded, 1.1f, true);
        }
    }
}

void AAPLevelStreamingVolume::CheckAllActorsLoaded()
{
    bool bAllActorsReady = true;
    for (AActor* Actor : StreamingLevel->GetLoadedLevel()->Actors)
    {
        if (Actor && !Actor->HasActorBegunPlay())
        {
            bAllActorsReady = false;
            break;
        }
    }

    if (bAllActorsReady)
    {
        // 모든 오브젝트가 스폰되었습니다.
        GetWorld()->GetTimerManager().ClearTimer(CheckActorsHandle);
        HideLoadingScreen();
    }
}

void AAPLevelStreamingVolume::HideLoadingScreen()
{
    auto APGI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); 
    if(APGI) APGI->OnStartFadeIn.Broadcast();
}
