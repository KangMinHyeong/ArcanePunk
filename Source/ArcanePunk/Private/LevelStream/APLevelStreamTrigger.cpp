
#include "LevelStream/APLevelStreamTrigger.h"

#include "Components/BoxComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"


AAPLevelStreamTrigger::AAPLevelStreamTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	LoadTriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("LoadTriggerVolume"));

	SetRootComponent(LoadTriggerVolume);
}

void AAPLevelStreamTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	LoadTriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AAPLevelStreamTrigger::LevelLoad);
	LoadTriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AAPLevelStreamTrigger::LeveUnlLoad);

	
}

#pragma region Level Streaming Trigger Func
void AAPLevelStreamTrigger::LevelLoad(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(IsLevelLoaded) return;
	auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	// Memory profiling - capture before load
	MemoryBeforeLoad = GetCurrentMemoryUsageMB();

	IsLevelLoaded = true;

	if(StreamingLevel.IsValid())
	{
		StreamingLevel.Get()->SetShouldBeVisible(IsLevelLoaded);

		// Log loading state
		LogLoadingState(TEXT("LOAD"));
	}

}

void AAPLevelStreamTrigger::LeveUnlLoad(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!IsLevelLoaded) return;
	auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	IsLevelLoaded = false;

	if(StreamingLevel.IsValid())
	{
		StreamingLevel.Get()->SetShouldBeVisible(IsLevelLoaded);

		// Log unloading state
		LogLoadingState(TEXT("UNLOAD"));
	}
}

float AAPLevelStreamTrigger::GetCurrentMemoryUsageMB() const
{
	FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
	return MemoryStats.UsedPhysical / (1024.0f * 1024.0f);
}

void AAPLevelStreamTrigger::LogLoadingState(const FString& Action) const
{
	if (!StreamingLevel.IsValid()) return;

	const float CurrentMemory = GetCurrentMemoryUsageMB();
	const float MemoryDelta = CurrentMemory - MemoryBeforeLoad;
	const FString LevelName = StreamingLevel->GetWorldAssetPackageFName().ToString();

	UE_LOG(LogTemp, Log, TEXT("[LevelStreaming] %s: %s | Priority: %d | Memory: %.2f MB (Delta: %+.2f MB)"),
		*Action, *LevelName, LoadingPriority, CurrentMemory, MemoryDelta);
}
#pragma endregion

void AAPLevelStreamTrigger::SetStreamingLevel(ULevelStreamingDynamic* Level)
{
	StreamingLevel = Level;
	
	TArray<AActor*> Players;
	LoadTriggerVolume->GetOverlappingActors(Players, AArcanePunkCharacter::StaticClass());
	
	if(StreamingLevel.IsValid() && Players.Num() > 0)
	{
		StreamingLevel.Get()->SetShouldBeVisible(true);
	}
}
