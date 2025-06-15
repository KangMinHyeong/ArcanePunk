
#include "GameElements/Volume/APBlockingVolume.h"

#include "Components/BoxComponent.h"

AAPBlockingVolume::AAPBlockingVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	BlockingVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockingVolume"));

	SetRootComponent(BlockingVolume);
}

void AAPBlockingVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAPBlockingVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

