// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Drop/Enemy_DropArcaneEnergy.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerState/ArcanePunkPlayerState.h"

AEnemy_DropArcaneEnergy::AEnemy_DropArcaneEnergy()
{

}

void AEnemy_DropArcaneEnergy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TWeakObjectPtr<AArcanePunkCharacter> PlayerCharacter = InteractTrigger->Character; if(!PlayerCharacter.IsValid()) return;

    FPlayerTotalData PD = PlayerCharacter->GetPlayerStatus();
    PD.PlayerDynamicData.MP = FMath::Min( PlayerCharacter->GetPlayerStatus().PlayerDynamicData.MP + MyArcaneEnergy, PlayerCharacter->GetPlayerStatus().PlayerDynamicData.MaxMP);
    PlayerCharacter->SetPlayerStatus(PD);

    Destroy();
}
