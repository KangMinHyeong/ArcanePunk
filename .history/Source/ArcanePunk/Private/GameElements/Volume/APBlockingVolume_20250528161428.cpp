
#include "GameElements/Volume/APBlockingVolume.h"

#include "Components/BoxComponent.h"

AAPBlockingVolume::AAPBlockingVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnMonsterComp"));

	SetRootComponent(TriggerVolume);
}

void AAPBlockingVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAPBlockingVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

