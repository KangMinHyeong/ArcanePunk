
#include "GameElements/Volume/APBlockingVolume.h"

#include "Components/BoxComponent.h"

AAPBlockingVolume::AAPBlockingVolume()
{
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAPBlockingVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAPBlockingVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

