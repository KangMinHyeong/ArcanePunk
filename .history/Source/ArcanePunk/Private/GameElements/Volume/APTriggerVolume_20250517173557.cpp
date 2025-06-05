
#include "GameElements/Volume/APTriggerVolume.h"

#include "Components/BoxComponent.h"

AAPTriggerVolume::AAPTriggerVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnMonsterComp"));

	SetRootComponent(TriggerVolume);
}

void AAPTriggerVolume::BeginPlay()
{
	Super::BeginPlay();
	
}



