// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/APTitleGameMode.h"

#include "PlayerState/ArcanePunkPlayerState.h"
#include "GameState/APGameState.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/APGameInstance.h"
#include "Components/AudioComponent.h"

AAPTitleGameMode::AAPTitleGameMode()
{
}

void AAPTitleGameMode::PostLogin(APlayerController *NewPlayer)
{
    Super::PostLogin(NewPlayer);

    // AArcanePunkPlayerState* MyPlayerState = Cast<AArcanePunkPlayerState>(NewPlayer->PlayerState);
	// if(!MyPlayerState) return;

    // AAPGameState* MyGameState = Cast<AAPGameState>(UGameplayStatics::GetGameState(this));
	// if(!MyGameState) return;
    
	// MyPlayerState->InitPlayerData();
    // MyGameState->InitGameData();
}

void AAPTitleGameMode::StartPlay()
{
    Super::StartPlay();

	auto APGI = Cast<UAPSoundSubsystem>(GetWorld()->GetGameInstance()->GetSubsystemBase(UAPSoundSubsystem::StaticClass())); if(!APGI) return;
    APGI->OnChangingSoundVolume.RemoveAll(this);
    APGI->OnChangingSoundVolume.AddUObject(this, &AAPTitleGameMode::OnChangingBGMVolume);

    if(BGM_Cue)
    GameMode_BGM = UAPSoundSubsystem::SpawnBGMSoundAtLocation(UAPGameInstance::GetSoundGI(GetWorld()), BGM_Cue);
}

void AAPTitleGameMode::OnChangingBGMVolume(float Master, float BGM, float Effect, float UI)
{
    if(!GameMode_BGM.IsValid()) return;

    GameMode_BGM->SetVolumeMultiplier(FMath::Max(0.001f,Master*BGM));
}