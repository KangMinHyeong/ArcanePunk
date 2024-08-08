// Fill out your copyright notice in the Description page of Project Settings.


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
#include "Components/Character/APAuraComponent.h"
#include "EngineUtils.h"
#include "GameElements/Trigger/APLimitCameraArea.h"

APortal_Normal::APortal_Normal()
{
	Destination = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destination"));
	Destination->SetupAttachment(PortalMesh);

	DestinationTrigger = CreateDefaultSubobject<UAPInteractionBoxComponent>(TEXT("DestinationTrigger"));
	DestinationTrigger->SetupAttachment(Destination);
}

void APortal_Normal::BeginFocus()
{
    TWeakObjectPtr<AArcanePunkCharacter> Character = PortalInteractionTrigger->Character; if(!Character.IsValid()) Character = DestinationTrigger->Character;
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
	UNiagaraFunctionLibrary::SpawnSystemAttached(PortalEffect, PlayerCharacter->GetMesh(), TEXT("PortalEffect"), PlayerLocation, PlayerCharacter->GetMesh()->GetComponentRotation(), EAttachLocation::KeepWorldPosition, true);
	SpawnSound(PlayerLocation);
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
    }
	
	CharacterPC = Cast<AArcanePunkPlayerController>(PlayerCharacter->GetController()); if(!CharacterPC.IsValid()) return;
	CharacterPC->StartFadeOut(1.5f, false);
	CharacterPC->OnEndedFadeOut.AddDynamic(this, &APortal_Normal::OnEndedFadeOut);
    PlayerCharacter->DisableInput(CharacterPC.Get());
	PlayerCharacter->SetActorEnableCollision(false);
}

void APortal_Normal::OnEndedFadeOut()
{
	if(!CharacterPC.IsValid()) return;
	auto Character = Cast<AArcanePunkCharacter>(CharacterPC->GetPawn()); if(!Character) return;
	Character->EnableInput(CharacterPC.Get());

	CharacterPC->OnEndedFadeOut.RemoveDynamic(this, &APortal_Normal::OnEndedFadeOut);

	StartTeleport(Character, Dest);
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
