
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
#include "GameElements/Volume/APLimitCameraVolume.h"
#include "Components/Character/APSpringArmComponent.h"

APortal_Normal::APortal_Normal()
{
	Destination = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destination"));
	Destination->SetupAttachment(PortalMesh);

	DestinationTrigger = CreateDefaultSubobject<UAPInteractionBoxComponent>(TEXT("DestinationTrigger"));
	DestinationTrigger->SetupAttachment(Destination);
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
	
	for(auto Area : TActorRange<AAPLimitCameraVolume>(GetWorld()))
    {
    	Area->AreaTrigger->OnComponentEndOverlap.RemoveDynamic(Area, &AAPLimitCameraVolume::OnOverlapEnd);
		Area->SetActorTickEnabled(false);
	}
	
	CharacterPC = Cast<AArcanePunkPlayerController>(PlayerCharacter->GetController()); if(!CharacterPC.IsValid()) return;

	UAPUserWidgetSubsystem::CreateFadeUI(this, true);

	auto Subsystem = UAPUserWidgetSubsystem::GetSubsystemSafe(this);
	if(Subsystem)
	{
		Subsystem->OnEndedFadeOut.AddDynamic(this, &APortal_Normal::OnEndedFadeOut);
		Subsystem->OnStartFadeIn.AddDynamic(this, &APortal_Normal::OnStartFadeIn);
	}
	
	PlayerCharacter->SetActorEnableCollision(false);
}

void APortal_Normal::OnEndedFadeOut()
{
	if(!CharacterPC.IsValid()) return;
	auto Character = Cast<AArcanePunkCharacter>(CharacterPC->GetPawn()); if(!Character) return;
	Character->SetActorEnableCollision(true);
	Character->DisableInput(CharacterPC.Get());

	StartTeleport(Character, Dest);

	UAPUserWidgetSubsystem::GetSubsystemSafe(this)->OnEndedFadeOut.RemoveDynamic(this, &APortal_Normal::OnEndedFadeOut);
}

void APortal_Normal::OnStartFadeIn()
{
	if(!CharacterPC.IsValid()) return;
	auto Character = Cast<AArcanePunkCharacter>(CharacterPC->GetPawn()); if(!Character) return;
	Character->EnableInput(CharacterPC.Get());
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	Character->GetAPSpringArm()->TargetOffset = FVector::ZeroVector;
	Destination->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UAPUserWidgetSubsystem::GetSubsystemSafe(this)->OnStartFadeIn.RemoveDynamic(this, &APortal_Normal::OnStartFadeIn);
}

void APortal_Normal::BeginPlay()
{
    Super::BeginPlay();

}

void APortal_Normal::StartTeleport(AArcanePunkCharacter* Character, const FVector & TeleportPoint)
{
	Super::StartTeleport(Character, TeleportPoint);	
}

