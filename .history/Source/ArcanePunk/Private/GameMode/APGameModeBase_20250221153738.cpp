
#include "GameMode/APGameModeBase.h"
#include "ArcanePunk/Public/Enemy/Enemy_CharacterBase.h"

#include "PlayerState/ArcanePunkPlayerState.h"
#include "GameState/APGameState.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/APGameInstance.h"
#include "Components/AudioComponent.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "UserInterface/HUD/APHUD.h"

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
	auto HUD = Cast<AAPHUD>(PC->GetHUD()); if(!HUD) return;

	for(auto Class : PlayerClass)
	{
		auto Player = GetWorld()->SpawnActor<AArcanePunkCharacter>(Class, FVector(10000.0f,10000.0f,10000.0f), FRotator::ZeroRotator);
		Player->SetActorHiddenInGame(true);  // 화면에서 숨김
        Player->SetActorEnableCollision(false);  // 충돌 비활성화
        Player->SetActorTickEnabled(false);  // 틱 비활성화
		
		
		Players.Emplace(Player);
	}

	uint8 PlayerIndex = 0; 
	for(auto Player : Players)
	{
		HUD->BindStatusBar(Player, PlayerIndex++);
	}
	
	
	if (MainPlayerIndex < Players.Num())
    {		
		auto MainPlayer = Cast<AArcanePunkCharacter>(Players[MainPlayerIndex]);

        PC->Possess(MainPlayer);
		
		auto PlayerStart = FindPlayerStart(PC);
		if(PlayerStart)
		{
			MainPlayer->SetActorLocation(PlayerStart->GetActorLocation());
			MainPlayer->SetActorRotation(PlayerStart->GetActorRotation());
		}
		
		MainPlayer->SetActorHiddenInGame(false); 
        MainPlayer->SetActorEnableCollision(true);  // 충돌 활성화
        MainPlayer->SetActorTickEnabled(true);  // 틱 활성화
		MainPlayer->InitPlayer();
    }
}