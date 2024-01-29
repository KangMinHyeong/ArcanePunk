
#include "LevelStream/APLevelStreamTrigger.h"

#include "Components/BoxComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"

AAPLevelStreamTrigger::AAPLevelStreamTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	LoadTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("LoadTrigger"));

	SetRootComponent(LoadTrigger);
}

void AAPLevelStreamTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	LoadTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAPLevelStreamTrigger::LevelLoad);
	LoadTrigger->OnComponentEndOverlap.AddDynamic(this, &AAPLevelStreamTrigger::LeveUnlLoad);
}

void AAPLevelStreamTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAPLevelStreamTrigger::LevelLoad(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(IsLevelLoad) return;
	auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	// FString levelName = TEXT("/Script/Engine.World'/Game/Maps/TestMap1_Sub/TestMap1_Stream2.TestMap1_Stream2'");
	FString levelName = TEXT("/Game/Maps/TestMap1_Sub/TestMap1_Stream2");

	FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(this, *levelName, true, true, LatentInfo);
	IsLevelLoad = true;
}

void AAPLevelStreamTrigger::LeveUnlLoad(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!IsLevelLoad) return;
	auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	FString levelName = TEXT("/Game/Maps/TestMap1_Sub/TestMap1_Stream2");

	FLatentActionInfo LatentInfo;
	UGameplayStatics::UnloadStreamLevel(this, *levelName, LatentInfo, false);
	IsLevelLoad = false;
}