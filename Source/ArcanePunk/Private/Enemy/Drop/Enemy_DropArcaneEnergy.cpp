// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Drop/Enemy_DropArcaneEnergy.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerState/ArcanePunkPlayerState.h"
#include "GameFramework/ProjectileMovementComponent.h"

AEnemy_DropArcaneEnergy::AEnemy_DropArcaneEnergy()
{
    DropMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ImpactMovementComponent"));
	DropMovement->MaxSpeed = DropSpeed;
	DropMovement->InitialSpeed = DropSpeed;
}

void AEnemy_DropArcaneEnergy::DropOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    auto PlayerCharacter = Cast<AArcanePunkCharacter>(OtherActor);
	if(!PlayerCharacter) return;

    FPlayerData PD = PlayerCharacter->GetPlayerStatus();
    PD.MP = PlayerCharacter->GetPlayerStatus().MP + MyArcaneEnergy;
    PlayerCharacter->SetPlayerStatus(PD);

    Destroy();
}
