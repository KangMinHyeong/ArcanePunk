
#include "Skill/Sub/APFreezing.h"

AAPFreezing::AAPFreezing()
{
	PrimaryActorTick.bCanEverTick = true;

	FreezingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FreezingMesh"));
	SetRootComponent(FreezingMesh);
}

void AAPFreezing::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorScale3D(FVector::ZeroVector);
}

void AAPFreezing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto Current = GetActorScale3D();
	Current = FMath::VInterpTo(Current, TargetScale, DeltaTime, TargetSpeed);
	SetActorScale3D(Current);
}

