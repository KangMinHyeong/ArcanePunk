// Fill out your copyright notice in the Description page of Project Settings.


#include "APGameModeBase.h"
#include "ArcanePunk/Public/Enemy/Enemy_CharacterBase.h"

#include "PlayerState/ArcanePunkPlayerState.h"
#include "GameState/APGameState.h"
#include "Kismet/GameplayStatics.h"

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

void AAPGameModeBase::SpawnMonster()
{
	FString BlueprintPath = TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Enemy/BP_Enemy_CharacterBase.BP_Enemy_CharacterBase_C'");
	UClass* BPClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *BlueprintPath));
	FString ControllerPath = TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Controller/Enemy/BP_EnemyBaseAIController.BP_EnemyBaseAIController_C'");
	UClass* ControllerClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *ControllerPath));

	FVector SpawnLocation = FVector(FMath::FRandRange(-2830.0f, 450.0f), FMath::FRandRange(-6050.0f, 1786.0f), FMath::FRandRange(1310.0f, 1400.0f));
	FRotator SpawnRotation = FRotator(0.0f, FMath::FRandRange(-180.0f, 180), 0.0f);

	AEnemy_CharacterBase* NewEnemy = GetWorld()->SpawnActor<AEnemy_CharacterBase>(BPClass, SpawnLocation, SpawnRotation);

	if (ControllerClass)
	{
		AController* NewController = GetWorld()->SpawnActor<AController>(ControllerClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (NewController)
		{
			NewEnemy->Controller = NewController;
			NewController->Possess(NewEnemy);
		}
	}
}

void AAPGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(MonsterSpawnTimerHandle, this, &AAPGameModeBase::SpawnMonster, SpawnInterval, true);
}