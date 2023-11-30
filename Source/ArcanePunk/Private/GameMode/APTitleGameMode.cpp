// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/APTitleGameMode.h"

#include "PlayerState/ArcanePunkPlayerState.h"
#include "GameState/APGameState.h"
#include "Kismet/GameplayStatics.h"

AAPTitleGameMode::AAPTitleGameMode()
{
}

void AAPTitleGameMode::PostLogin(APlayerController *NewPlayer)
{
    Super::PostLogin(NewPlayer);

    AArcanePunkPlayerState* MyPlayerState = Cast<AArcanePunkPlayerState>(NewPlayer->PlayerState);
	if(!MyPlayerState) return;

    AAPGameState* MyGameState = Cast<AAPGameState>(UGameplayStatics::GetGameState(this));
	if(!MyGameState) return;
    
	MyPlayerState->InitPlayerData();
    MyGameState->InitGameData();
}