
#include "Interaction/APInteractionTriggerBase.h"

#include "Camera/CameraComponent.h"
#include "Character/ArcanePunkCharacter.h"

AAPInteractionTriggerBase::AAPInteractionTriggerBase()
{
	PrimaryActorTick.bCanEverTick = false;

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


