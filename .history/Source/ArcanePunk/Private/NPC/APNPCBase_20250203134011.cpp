
#include "NPC/APNPCBase.h"

#include "Interaction/APInteractionTriggerBase.h"

AAPNPCBase::AAPNPCBase()
{
	PrimaryActorTick.bCanEverTick = false;

	DestinationComp = CreateDefaultSubobject<USceneComponent>(TEXT("DestinationComp"));
	DestinationComp->SetupAttachment(GetRootComponent());

	DialogueWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("DialogueWidgetComp"));
	DialogueWidgetComp->SetupAttachment(GetCapsuleComponent());
	DialogueWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DialogueWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
}

void AAPNPCBase::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnInteractionTrigger();
}

void AAPNPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAPNPCBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAPNPCBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AAPNPCBase::SpawnInteractionTrigger()
{
	Interaction = GetWorld()->SpawnActor<AAPInteractionTriggerBase>(InteractionTrigger, GetActorTransform());
	if(!Interaction.IsValid()) return;
	Interaction->SetOwner(this);
	Interaction->SetDialogoueGroupID(NPCID);
	Interaction->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
}

void AAPNPCBase::InteractionCollisionEnable(bool NewBool)
{
	Interaction->SetActorEnableCollision(NewBool);	
}
