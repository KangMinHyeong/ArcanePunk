// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/Portal_Normal.h"

#include "Components/BoxComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

APortal_Normal::APortal_Normal()
{
    DestinationTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DestinationTrigger"));
	DestinationTrigger->SetupAttachment(PortalTrigger);

	Destination = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destination"));
	Destination->SetupAttachment(DestinationTrigger);
}

void APortal_Normal::BeginPlay()
{
    Super::BeginPlay();

    if(BothSides) DestinationTrigger->OnComponentBeginOverlap.AddDynamic(this, &APortal_Normal::OnTeleport_B);

}

void APortal_Normal::OnTeleport_A(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    Start = true;
    Super::OnTeleport_A(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void APortal_Normal::OnTeleport_B(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	Start = false;

	OverlapCharacter = Cast<AArcanePunkCharacter>(OtherActor);
	if(!OverlapCharacter) return;
	OverlapCharacter->SetCanMove(false);
	GetWorldTimerManager().SetTimer(Delay_TimerHandle, this, &APortal_Normal::StartTeleport, Delay_Time, false);
}

void APortal_Normal::StartTeleport()
{
    if(OverlapCharacter->ActorHasTag(PlayerTag))
	{
		if(Start) OverlapCharacter->SetActorLocation(DestinationTrigger->GetComponentLocation());
		else OverlapCharacter->SetActorLocation(PortalTrigger->GetComponentLocation());
		
		OverlapCharacter->SetCanMove(true);
		GetWorldTimerManager().ClearTimer(Delay_TimerHandle);
	}
}
