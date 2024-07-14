
#include "Components/Common/APManaDropComponent.h"

#include "GameElements/Drop/APManaEnergy.h"

UAPManaDropComponent::UAPManaDropComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAPManaDropComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UAPManaDropComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAPManaDropComponent::SpawnManaDrop(AActor* ManaOwner)
{
	float Check = FMath::RandRange(0.0f, 100.0f);
	if(Check > DropPercent) return;

	auto ManaDrop = GetWorld()->SpawnActor<AAPManaEnergy>(ManaDropClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	if(!ManaDrop) return;
	ManaDrop->SetOwner(GetOwner());
	ManaDrop->SetEnergyMoveComp(ManaOwner);
}
