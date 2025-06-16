
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
	
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AAPTrapBase::OnOverlap);
}

void AAPTriggerVolume::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}



