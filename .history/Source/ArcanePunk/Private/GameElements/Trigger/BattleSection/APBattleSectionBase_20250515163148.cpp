
#include "GameElements/Trigger/BattleSection/APBattleSectionBase.h"

#include "Enemy/Enemy_BossBase.h"
#include "Components/BoxComponent.h"
#include "Components/Common/APSpawnMonsterComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "GameElements/SpawnPoint/APSpawnPointBase.h"
#include "GameState/APGameState.h"
#include "GameMode/APGameModeBattleStage.h"
#include "Kismet/GameplayStatics.h"
#include "UserInterface/Stage/Battle/APBattleStageUI.h"

AAPBattleSectionBase::AAPBattleSectionBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnMonsterComp = CreateDefaultSubobject<UAPSpawnMonsterComponent>(TEXT("SpawnMonsterComp"));
    SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));

	SetRootComponent(SpawnVolume);
}

void AAPBattleSectionBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitArea();
	// ClearChecking();
	GetWorldTimerManager().SetTimer(TriggerTimerHandle, this, &AAPBattleSectionBase::ClearChecking, 0.1f, false);
}

void AAPBattleSectionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AAPBattleSectionBase::InitArea()
{
	auto AreaDegree = GetActorRotation().Yaw;
	AreaDegree = FMath::DegreesToRadians(AreaDegree);
	auto AreaWidth = SpawnVolume->GetScaledBoxExtent().X * 2.0f;
	auto AreaHeight = SpawnVolume->GetScaledBoxExtent().Y * 2.0f;
	auto AreaCenter = GetActorLocation();
	float cos = FMath::Cos(AreaDegree); float sin = FMath::Sin(AreaDegree);
	float w = AreaWidth * 0.5f; float h = AreaHeight * 0.5f;
	AreaPoint_1 = FVector(AreaCenter.X + cos * (w) + sin * (h), AreaCenter.Y -(- sin * (w) + cos * (h)), AreaCenter.Z);
	AreaPoint_2 = FVector(AreaCenter.X + cos * (-w) + sin * (h), AreaCenter.Y -(- sin * (-w) + cos * (h)), AreaCenter.Z);
	AreaPoint_3 = FVector(AreaCenter.X + cos * (-w) + sin * (-h), AreaCenter.Y -(- sin * (-w) + cos * (-h)), AreaCenter.Z);
	AreaPoint_4 = FVector(AreaCenter.X + cos * (w) + sin * (-h), AreaCenter.Y -(- sin * (w) + cos * (-h)), AreaCenter.Z);
}


void AAPBattleSectionBase::ClearChecking() // 인스턴스 던전에서 해당 배틀 섹션을 클리어 하였는지 체크 & BattleStage Information 정보들 초기화
{
	auto GS = Cast<AAPGameState>(GetWorld()->GetGameState()); if(!GS) return;
	SpawnData = SpawnDataTable->FindRow<FSpawnerData>(SpawnDataID, SpawnDataID.ToString()); if(!SpawnData) return;

	TArray<AActor*> Players;
	SpawnVolume->GetOverlappingActors(Players, AArcanePunkCharacter::StaticClass());
	
	if(!Players.IsEmpty()) OnSpawnLocation();
}

void AAPBattleSectionBase::OnSpawnLocation()
{
	if(!SpawnData) return;

	BattleUI = CreateWidget<UAPBattleStageUI>(GetWorld(), BattleUIClass);
	if(BattleUI.IsValid()) 
	{
		BattleUI->AddToViewport(-1);
		BattleUI->UpdateBattleUI(SpawnData->SpawnStep.Num() - SpawnStepCount, SpawnMonsterTime);
		BattleUI->OnStartingNewWave.AddUObject(this, &AAPBattleSectionBase::SpawnMonster);
	}
}

void AAPBattleSectionBase::SpawnMonster()
{
	if(!SpawnData) return;

	if(SpawnStepCount < SpawnData->SpawnStep.Num())
	{
		for( auto SpawnEntry : SpawnData->SpawnStep[SpawnStepCount].SpawnEntry)
		{
			if (SpawnEntry.SpawnEntity->IsChildOf(AEnemy_BossBase::StaticClass()))
			{
				SpawnMonsterComp->SpawnMonsterRandomWithLocation(SpawnEntry.SpawnEntity, SpawnEntry.EntityQuantity, GetActorLocation());
				continue;
			}
			
			SpawnMonsterComp->SpawnMonsterRandomWithSquare(SpawnEntry.SpawnEntity, SpawnEntry.EntityQuantity, AreaPoint_1, AreaPoint_2, AreaPoint_3, AreaPoint_4);
		}

		SpawnStepCount++;
		if(BattleUI.IsValid()) BattleUI->UpdateBattleUI(SpawnData->SpawnStep.Num() - SpawnStepCount, SpawnData->SpawnStep[SpawnStepCount - 1]->TimerValue);
	}
}

bool AAPBattleSectionBase::CheckSpawnEnd()
{
	// if(!bOvelapped) return false;
	SpawnData = SpawnDataTable->FindRow<FSpawnerData>(SpawnDataID, SpawnDataID.ToString()); if(!SpawnData)return false;


	if(SpawnStepCount < SpawnData->SpawnStep.Num()) return false;

	if(SpawnNum_Complete < BattleStageData->Spawn_Complete.Num())
	{
		for( auto Spawn : BattleStageData->Spawn_Complete[SpawnNum_Complete].SpawnMonsterData)
		{
			bool Check = FMath::RandBool();
			if (Check)
			{
				SpawnMonsterComp->SpawnMonsterRandomWithTriangle(Spawn.Key, Spawn.Value, AreaPoint_1, AreaPoint_2, AreaPoint_3);
			}
			else
			{
				SpawnMonsterComp->SpawnMonsterRandomWithTriangle(Spawn.Key, Spawn.Value, AreaPoint_3, AreaPoint_4, AreaPoint_1);
			}
		}
		SpawnNum_Complete++;
		
		return false; 
	}

	if(BattleUI.IsValid()) BattleUI->RemoveFromParent();
	return true;
}
