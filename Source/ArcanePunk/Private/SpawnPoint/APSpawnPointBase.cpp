
#include "SpawnPoint/APSpawnPointBase.h"

#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"

AAPSpawnPointBase::AAPSpawnPointBase()
{
	PrimaryActorTick.bCanEverTick = false;

	BlockingArea = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BlockingArea"));
	SpawnPointEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpawnPointEffect"));
	PanelComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PanelComp"));

	SetRootComponent(BlockingArea);
	SpawnPointEffect->SetupAttachment(BlockingArea);
	PanelComp->SetupAttachment(BlockingArea);
	
}

void AAPSpawnPointBase::BeginPlay()
{
	Super::BeginPlay();
	
	DefaultVector = GetActorLocation();
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, BlockingArea->GetScaledCapsuleHalfHeight()));
}

void AAPSpawnPointBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

