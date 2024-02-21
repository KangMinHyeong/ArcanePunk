
#include "Interaction/APInteractionTriggerBase.h"

#include "Components/BoxComponent.h"

AAPInteractionTriggerBase::AAPInteractionTriggerBase()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionTrigger"));

	SetRootComponent(InteractionTrigger);
}

void AAPInteractionTriggerBase::BeginPlay()
{
	Super::BeginPlay();
	
	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAPInteractionTriggerBase::OnInteractionTrigger);
}

void AAPInteractionTriggerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAPInteractionTriggerBase::OnInteractionTrigger(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

}
