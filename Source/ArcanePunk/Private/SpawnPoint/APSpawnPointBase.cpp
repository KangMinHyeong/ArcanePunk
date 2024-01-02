
#include "SpawnPoint/APSpawnPointBase.h"

#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"

AAPSpawnPointBase::AAPSpawnPointBase()
{
	PrimaryActorTick.bCanEverTick = false;

	BlockingArea = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BlockingArea"));
	SetRootComponent(BlockingArea);

	SpawnPointEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpawnPointEffect"));
	SpawnPointEffect->SetupAttachment(BlockingArea);
}

void AAPSpawnPointBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, BlockingArea->GetScaledCapsuleHalfHeight()));
}

void AAPSpawnPointBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

