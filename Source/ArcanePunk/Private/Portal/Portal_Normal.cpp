// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/Portal_Normal.h"

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

	if((PlayerLocation - PortalInteractionTrigger->GetComponentLocation()).Size() < (PlayerLocation - DestinationTrigger->GetComponentLocation()).Size())
	{
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &APortal_Normal::StartTeleport, PlayerCharacter, DestinationTrigger->GetComponentLocation());
		GetWorld()->GetTimerManager().SetTimer(Delay_TimerHandle, TimerDelegate, Delay_Time, false);
	}
	else
	{
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &APortal_Normal::StartTeleport, PlayerCharacter, PortalInteractionTrigger->GetComponentLocation());
		GetWorld()->GetTimerManager().SetTimer(Delay_TimerHandle, TimerDelegate, Delay_Time, false);
	}

	auto CharacterPC = Cast<AArcanePunkPlayerController>(PlayerCharacter->GetController()); if(!CharacterPC) return;
    PlayerCharacter->DisableInput(CharacterPC);
}

void APortal_Normal::BeginPlay()
{
    Super::BeginPlay();

}

void APortal_Normal::StartTeleport(AArcanePunkCharacter* Character, FVector TeleportPoint)
{
	Super::StartTeleport(Character, TeleportPoint);
	Character->GetPlayerPanelAura()->SetHiddenInGame(false);

	auto CharacterPC = Cast<AArcanePunkPlayerController>(Character->GetController()); if(!CharacterPC) return;
    Character->EnableInput(CharacterPC);
}

void APortal_Normal::SpawnSound(FVector Location)
{
	Super::SpawnSound(Location);
} 
