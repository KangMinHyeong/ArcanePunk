
#include "Components/Common/APManaDropComponent.h"

#include "GameElements/Drop/APManaEnergy.h"
#include "GameInstance/APGameInstance.h"

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
	if(Check >= DropPercent) return;

	auto GI = Cast<UAPGameInstance>(GetOwner()->GetGameInstance()); if(!GI) return;

	uint8 Amount = ManaAmount;
	while(Amount >= 1)
	{
		Amount--;
		auto ManaDrop = GetWorld()->SpawnActor<AAPManaEnergy>(GI->GetManaEnergyClass(), GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
		if(!ManaDrop) continue;;
		ManaDrop->SetOwner(GetOwner());
		ManaDrop->SetEnergyMoveComp(ManaOwner);
	}
}
