
#include "NPC/APNPCBase.h"

#include "Interaction/APInteractionTriggerBase.h"
#include "Components/WidgetComponent.h"
#include "UserInterface/Common/WidgetComponent/APTextWidgetComponent.h"
#include "GameMode/APGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/ArcanePunkPlayerState.h"

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
	


	auto PS = Cast<AArcanePunkPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0)); if(!PS) return;
	auto DeathCount = PS->PlayerTotalStatus.PlayerDeathCount;

	if(ID_Map.Contains(DeathCount)) {NPCID = ID_Map[DeathCount];}
	else 
	{
		if(ID_Map.Contains(0)) {NPCID = ID_Map[0];}
		else {NPCID = 0;}
	}

	UE_LOG(LogTemp, Display, TEXT("Your NPCID %d"), NPCID);

	SpawnInteractionTrigger();
	
	DialogueUI = Cast<UAPTextWidgetComponent>(DialogueWidgetComp->GetUserWidgetObject());
	if(DialogueUI.IsValid()) DialogueUI->SetDialogueText();

	auto GM = Cast<AAPGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
    if(GM) GM->OnEndedConversation.AddDynamic(this, &AAPNPCBase::CheckDialogueUI);
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