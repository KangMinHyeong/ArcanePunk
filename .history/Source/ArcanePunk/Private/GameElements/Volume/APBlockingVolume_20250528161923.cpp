
#include "GameElements/Volume/APBlockingVolume.h"

#include "Components/BoxComponent.h"

AAPBlockingVolume::AAPBlockingVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	BlockingVolum = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockingVolum"));

	SetRootComponent(BlockingVolum);
}

void AAPBlockingVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAPBlockingVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

