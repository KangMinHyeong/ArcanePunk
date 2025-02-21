
#include "GameMode/APGameModeBase.h"
#include "ArcanePunk/Public/Enemy/Enemy_CharacterBase.h"

#include "PlayerState/ArcanePunkPlayerState.h"
#include "GameState/APGameState.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/APGameInstance.h"
#include "Components/AudioComponent.h"

AAPGameModeBase::AAPGameModeBase()
{
}

void AAPGameModeBase::PostLogin(APlayerController *NewPlayer)
{
    Super::PostLogin(NewPlayer);

    AArcanePunkPlayerState* MyPlayerState = Cast<AArcanePunkPlayerState>(NewPlayer->PlayerState);
	if(!MyPlayerState) return;

    AAPGameState* MyGameState = Cast<AAPGameState>(UGameplayStatics::GetGameState(this));
	if(!MyGameState) return;
    
	MyPlayerState->InitPlayerData();
    MyGameState->InitGameData();
}

void AAPGameModeBase::StartPlay()
{
	Super::StartPlay();

	auto APGI = Cast<UAPSoundSubsystem>(GetWorld()->GetGameInstance()->GetSubsystemBase(UAPSoundSubsystem::StaticClass())); if(!APGI) return;
    APGI->OnChangingSoundVolume.RemoveAll(this);
    APGI->OnChangingSoundVolume.AddUObject(this, &AAPGameModeBase::OnChangingBGMVolume);

	if(BGM_Cue)
    GameMode_BGM = UAPSoundSubsystem::SpawnBGMSoundAtLocation(UAPGameInstance::GetSoundGI(GetWorld()), BGM_Cue);

	SpawnPlayers();
}

void AAPGameModeBase::OnChangingBGMVolume(float Master, float BGM, float Effect, float UI)
{
	if(!GameMode_BGM.IsValid()) return;
    GameMode_BGM->SetVolumeMultiplier(FMath::Max(Master*BGM, 0.001f));
}

void AAPGameModeBase::SpawnPlayers()
{
	auto PC = GetWorld()->GetFirstPlayerController(); if(!PC) return;
	
	
	for(auto Class : PlayerClass)
	{
		auto Player = GetWorld()->SpawnActor<AArcanePunkCharacter>(Class, FVector(10000.0f,10000.0f,10000.0f), SpawnRotation);
		Player->SetActorHiddenInGame(true);  // 화면에서 숨김
        Player->SetActorEnableCollision(false);  // 충돌 비활성화
        Player->SetActorTickEnabled(false);  // 틱 비활성화

		Players.Emplace(Player);
	}

	

	if (Players.Num() <= MainPlayerIndex)
    {
		Players[MainPlayerIndex]->SetActorHiddenInGame(true);  // 화면에서 숨김
        Players[MainPlayerIndex]->SetActorEnableCollision(false);  // 충돌 비활성화
        Players[MainPlayerIndex]->SetActorTickEnabled(false);  // 틱 비활성화
		FindPlayerStart
        PC->Possess(Players[MainPlayerIndex]);
    }
}