
#include "GameElements/Volume/APTriggerVolume.h"

AAPTriggerVolume::AAPTriggerVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnMonsterComp"));

	SetRootComponent(TriggerVolume);

}

// Called when the game starts or when spawned
void AAPTriggerVolume::BeginPlay()
{
	Super::BeginPlay();
	
}



