
#include "Interaction/APInteractionTriggerBase.h"

#include "Camera/CameraComponent.h"
#include "Character/ArcanePunkCharacter.h"

AAPInteractionTriggerBase::AAPInteractionTriggerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractionTrigger = CreateDefaultSubobject<UAPInteractionBoxComponent>(TEXT("InteractionTrigger"));
	InteractionCamera =  CreateDefaultSubobject<UCameraComponent>(TEXT("InteractionCamera"));

	SetRootComponent(InteractionTrigger);
	InteractionCamera->SetupAttachment(InteractionTrigger);
}

void AAPInteractionTriggerBase::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
}

void AAPInteractionTriggerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FInteractData AAPInteractionTriggerBase::GetInteractData()
{
    return InteractionTrigger->GetInteractionData();
}

void AAPInteractionTriggerBase::Interact(AArcanePunkCharacter *PlayerCharacter)
{
    if(!PlayerCharacter) return;
    
    PlayerCharacter->SetInteraction(false);
    PlayerCharacter->InteractionActorRemove(this);
}


