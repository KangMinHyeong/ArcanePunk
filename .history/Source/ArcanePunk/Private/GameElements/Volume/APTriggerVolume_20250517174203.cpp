
#include "GameElements/Volume/APTriggerVolume.h"

#include "Components/BoxComponent.h"
#include "GameInstance/APGameInstance.h"

AAPTriggerVolume::AAPTriggerVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnMonsterComp"));

	SetRootComponent(TriggerVolume);
}

void AAPTriggerVolume::BeginPlay()
{
	Super::BeginPlay();
	
	Init();
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AAPTrapBase::OnOverlap);
}

void Init()
{
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AAPTrapBase::OnOverlap);

	APGI = Cast<UAPGameInstance>(GetGameInstance());
}

void AAPTriggerVolume::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!APGI.IsValid()) return;

	
}



