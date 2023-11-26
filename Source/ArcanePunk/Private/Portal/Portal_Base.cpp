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

}

// Called when the game starts or when spawned
void APortal_Base::BeginPlay()
{
	Super::BeginPlay();
	
	PortalTrigger->OnComponentBeginOverlap.AddDynamic(this, &APortal_Base::OnTeleport_A);
}

void APortal_Base::OnTeleport_A(UPrimitiveComponent*OverlappedComp, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlapCharacter = Cast<AArcanePunkCharacter>(OtherActor);
	if(!OverlapCharacter) return;
	OverlapCharacter->SetCanMove(false);
	GetWorldTimerManager().SetTimer(Delay_TimerHandle, this, &APortal_Base::StartTeleport, Delay_Time, false);	
}

void APortal_Base::StartTeleport()
{
	OverlapCharacter->SetCanMove(true);
	GetWorldTimerManager().ClearTimer(Delay_TimerHandle);
}
