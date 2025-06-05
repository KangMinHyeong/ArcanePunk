
#include "GameElements/Volume/APTriggerVolume.h"

#include "Components/BoxComponent.h"
#include "GameInstance/APGameInstance.h"
#include "GameElements/Volume/SpawnVolume/APSpawnVolume.h"
#include "Character/ArcanePunkCharacter.h"

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
}

void AAPTriggerVolume::Init()
{
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AAPTriggerVolume::OnOverlap);

	APGI = Cast<UAPGameInstance>(GetGameInstance());
}

void AAPTriggerVolume::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Player = Cast<AArcanePunkCharacter>(OtherActor); 
	if(!Player || !APGI.IsValid()) return;	
	
	TriggerVolume->OnComponentBeginOverlap.RemoveDynamic(this, &AAPTriggerVolume::OnOverlap);

	auto SpawnVolume = APGI->FindSpawnVolume(TriggerID);
	if(!SpawnVolume) return;
	
	SpawnVolume->StartSpawn();
}



