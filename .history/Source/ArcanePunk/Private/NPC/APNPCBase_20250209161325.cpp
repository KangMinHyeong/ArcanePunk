
#include "NPC/APNPCBase.h"

#include "Interaction/APInteractionTriggerBase.h"
#include "Components/WidgetComponent.h"
#include "UserInterface/Common/WidgetComponent/APTextWidgetComponent.h"
#include "GameMode/APGameModeBase.h"
#include "Kismet/GameplayStatics.h"

AAPNPCBase::AAPNPCBase()
{
	PrimaryActorTick.bCanEverTick = false;

	DestinationComp = CreateDefaultSubobject<USceneComponent>(TEXT("DestinationComp"));
	DestinationComp->SetupAttachment(GetRootComponent());

	DialogueWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("DialogueWidgetComp"));
	DialogueWidgetComp->SetupAttachment(GetRootComponent());
	DialogueWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DialogueWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
}

void AAPNPCBase::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnInteractionTrigger();

	DialogueUI = Cast<UAPTextWidgetComponent>(DialogueWidgetComp->GetUserWidgetObject());
	if(DialogueUI.IsValid()) DialogueUI->SetDialogueText();

	auto GM = Cast<AAPGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
    if(GM) GM->OnEndedConversation.AddDynamic(this, &AAPNPCBase::CheckDialogueUI);

	auto GM = Cast<AAPGameModeBase>(UGameplayStatics::GetPlayerState());
	
	NPCID = ID_Map
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
	Interaction = GetWorld()->SpawnActor<AAPInteractionTriggerBase>(InteractionTrigger, GetMesh()->GetComponentLocation(), GetMesh()->GetComponentRotation());
	if(!Interaction.IsValid()) return;
	Interaction->SetOwner(this);
	Interaction->SetDialogoueGroupID(NPCID);
	Interaction->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
}

void AAPNPCBase::InteractionCollisionEnable(bool NewBool)
{
	Interaction->SetActorEnableCollision(NewBool);	
}

void AAPNPCBase::CheckDialogueUI(const int32 CheckID)
{
	if(CheckID != NPCID) return;

	DialogueWidgetComp->SetHiddenInGame(true);
}