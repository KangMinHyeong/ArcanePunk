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

    auto APGI = Cast<UAPGameInstance>(GetWorld()->GetGameInstance()); if(!APGI) return;
    APGI->OnChangingSoundVolume.RemoveAll(this);
    APGI->OnChangingSoundVolume.AddUObject(this, &AAPTitleGameMode::OnChangingBGMVolume);

    if(BGM_Cue)
    {
        float SoundVolume = APGI->GetGameSoundVolume().MasterVolume * APGI->GetGameSoundVolume().BGMVolume;
        GameMode_BGM = UGameplayStatics::SpawnSound2D(GetWorld(), BGM_Cue, SoundVolume);      
    }
}

void AAPTitleGameMode::OnChangingBGMVolume(float Master, float BGM, float Effect)
{
    if(!GameMode_BGM.IsValid()) return;

    GameMode_BGM->AdjustVolume(0.1f, FMath::Max(0.001f,Master*BGM));
}