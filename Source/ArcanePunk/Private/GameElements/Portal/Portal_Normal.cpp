
#include "GameElements/Portal/Portal_Normal.h"

#include "Components/Common/APInteractionBoxComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/APGameInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "EngineUtils.h"
#include "GameElements/Trigger/APLimitCameraArea.h"
#include "Components/Character/APSpringArmComponent.h"

APortal_Normal::APortal_Normal()
{
	Destination = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destination"));
	Destination->SetupAttachment(PortalMesh);

	DestinationTrigger = CreateDefaultSubobject<UAPInteractionBoxComponent>(TEXT("DestinationTrigger"));
	DestinationTrigger->SetupAttachment(Destination);
}

void APortal_Normal::BeginFocus()
{
    TWeakObjectPtr<AArcanePunkCharacter> Character = PortalInteractionTrigger->Character; 
	if(!Character.IsValid() && BothSides) Character = DestinationTrigger->Character;
	if(!Character.IsValid()) return;
	Character->ActivateInteractionSweep();

    GetWorld()->GetTimerManager().SetTimer(InteractTimerHandle, this, &APortal_Normal::BeginFocus, InteractFrequency, true);
}

void APortal_Normal::EndFocus()
{
	GetWorld()->GetTimerManager().ClearTimer(InteractTimerHandle);
}

FInteractData APortal_Normal::GetInteractData()
{
    return PortalInteractionTrigger->GetInteractionData();
}

void APortal_Normal::Interact(AArcanePunkCharacter *PlayerCharacter)
{
	FVector PlayerLocation = PlayerCharacter->GetMesh()->GetComponentLocation();
	PlayerCharacter->SetCanMove(false);
	PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayerCharacter->GetPlayerPanelAura()->SetHiddenInGame(true);
	
	if(DestinationTrigger->IsOverlappingActor(PlayerCharacter))
	{
		Dest = PortalInteractionTrigger->GetComponentLocation();
	}
	else
	{
		Dest = DestinationTrigger->GetComponentLocation();
	}
	
	for(auto Area : TActorRange<AAPLimitCameraArea>(GetWorld()))
    {
    	Area->AreaTrigger->OnComponentEndOverlap.RemoveDynamic(Area, &AAPLimitCameraArea::OnOverlapEnd);
		Area->SetActorTickEnabled(false);
	}
	
	CharacterPC = Cast<AArcanePunkPlayerController>(PlayerCharacter->GetController()); if(!CharacterPC.IsValid()) return;
	CharacterPC->StartFadeOut(1.5f, false);
	APGI->OnEndedFadeOut.AddDynamic(this, &APortal_Normal::OnEndedFadeOut);
	APGI->OnStartFadeIn.AddDynamic(this, &APortal_Normal::OnStartFadeIn);
    PlayerCharacter->DisableInput(CharacterPC.Get());
	PlayerCharacter->SetActorEnableCollision(false);
	
}

void APortal_Normal::OnEndedFadeOut()
{
	if(!CharacterPC.IsValid()) return;
	auto Character = Cast<AArcanePunkCharacter>(CharacterPC->GetPawn()); if(!Character) return;
	Character->SetActorEnableCollision(true);
	StartTeleport(Character, Dest);

	APGI->OnEndedFadeOut.RemoveDynamic(this, &APortal_Normal::OnEndedFadeOut);
}

void APortal_Normal::OnStartFadeIn()
{
	if(!CharacterPC.IsValid()) return;
	auto Character = Cast<AArcanePunkCharacter>(CharacterPC->GetPawn()); if(!Character) return;
	Character->EnableInput(CharacterPC.Get());
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Character->GetAPSpringArm()->TargetOffset = FVector::ZeroVector;
	CharacterPC->StartFadeIn(1.0f, false);
	Destination->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	APGI->OnStartFadeIn.RemoveDynamic(this, &APortal_Normal::OnStartFadeIn);
}

void APortal_Normal::BeginPlay()
{
    Super::BeginPlay();

}

void APortal_Normal::StartTeleport(AArcanePunkCharacter* Character, FVector TeleportPoint)
{
	Super::StartTeleport(Character, TeleportPoint);	
}

void APortal_Normal::SpawnSound(FVector Location)
{
	Super::SpawnSound(Location);
} 
