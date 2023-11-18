// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/Portal_Base.h"

#include "Components/BoxComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

// Sets default values
APortal_Base::APortal_Base()
{
	PrimaryActorTick.bCanEverTick = false;

	PortalTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PortalTrigger"));
	SetRootComponent(PortalTrigger);

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	PortalMesh->SetupAttachment(PortalTrigger);

	DestinationTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DestinationTrigger"));
	DestinationTrigger->SetupAttachment(PortalTrigger);

	Destination = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destination"));
	Destination->SetupAttachment(DestinationTrigger);

}

// Called when the game starts or when spawned
void APortal_Base::BeginPlay()
{
	Super::BeginPlay();
	
	PortalTrigger->OnComponentBeginOverlap.AddDynamic(this, &APortal_Base::OnTeleport_A);
	if(BothSides) DestinationTrigger->OnComponentBeginOverlap.AddDynamic(this, &APortal_Base::OnTeleport_B);
}

void APortal_Base::OnTeleport_A(UPrimitiveComponent*OverlappedComp, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Start = true;

	OverlapCharacter = Cast<AArcanePunkCharacter>(OtherActor);
	if(!OverlapCharacter) return;
	OverlapCharacter->SetCanMove(false);
	GetWorldTimerManager().SetTimer(Delay_TimerHandle, this, &APortal_Base::StartTeleport, Delay_Time, false);	
}

void APortal_Base::OnTeleport_B(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	Start = false;

	OverlapCharacter = Cast<AArcanePunkCharacter>(OtherActor);
	if(!OverlapCharacter) return;
	OverlapCharacter->SetCanMove(false);
	GetWorldTimerManager().SetTimer(Delay_TimerHandle, this, &APortal_Base::StartTeleport, Delay_Time, false);
}

void APortal_Base::StartTeleport()
{
	if(OverlapCharacter->ActorHasTag(PlayerTag))
	{
		if(Start) OverlapCharacter->SetActorLocation(DestinationTrigger->GetComponentLocation());
		else OverlapCharacter->SetActorLocation(PortalTrigger->GetComponentLocation());
		
		OverlapCharacter->SetCanMove(true);
		GetWorldTimerManager().ClearTimer(Delay_TimerHandle);
	}
}
