
#include "GameElements/Volume/SpawnVolume/APSpawnVolume.h"

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
#include "GameInstance/APGameInstance.h"
#include "NavModifierComponent.h"
#include "AI/NavigationModifier.h"
#include "NavAreas/NavArea_Null.h"

AAPSpawnVolume::AAPSpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnMonsterComp = CreateDefaultSubobject<UAPSpawnMonsterComponent>(TEXT("SpawnMonsterComp"));
    
	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SetRootComponent(SpawnVolume);
	SpawnVolume->SetCanEverAffectNavigation(true); // NavMesh에 영향 미치도록 설정

	// NavModifierComponent 생성
	NavModifier = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifier"));
	NavModifier->SetAreaClass(nullptr); // 이 영역은 NavMesh에서 제외
}

void AAPSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	InitArea();
	InitData();
	RegisterSpawnVolume();
}

void AAPSpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AAPSpawnVolume::InitArea()
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

void AAPSpawnVolume::InitData() 
{
	SpawnData = SpawnDataTable->FindRow<FSpawnerData>(SpawnDataID, SpawnDataID.ToString()); if(!SpawnData) return;
}

void AAPSpawnVolume::RegisterSpawnVolume()
{
	auto GI = Cast<UAPGameInstance>(GetGameInstance()); if(!GI) return;

	GI->RegisterSpawnVolume(SpawnDataID, this);
}

void AAPSpawnVolume::StartSpawn()
{
	if(!SpawnData) return;

	bActive = true;

	for(FSpawnStep & Step : SpawnData->SpawnStep)
	{
		if(Step.SpawnStepStartCondition == ESpawnStepStartCondition::SpawnerActive)
		{
			SpawnStep = (&Step);
		}
		else if(Step.SpawnStepStartCondition == ESpawnStepStartCondition::PreviousSpawnStepFinish)
		{
			PreviousFinishSteps.Add({Step.PreviousSpawnStepID, &Step});
		}
	}

	SpawnMonster();	
}

void AAPSpawnVolume::ActivateBattleTimerUI()
{
	if(BattleUI.IsValid()) BattleUI->RemoveFromParent();

	BattleUI = CreateWidget<UAPBattleStageUI>(GetWorld(), BattleUIClass);
	if(BattleUI.IsValid()) 
	{
		BattleUI->AddToViewport(-1);
		BattleUI->UpdateBattleUI(1, SpawnStep->TimerValue);
		BattleUI->OnStartingNewWave.AddUObject(this, &AAPSpawnVolume::DespawnMonster);
	}
}

void AAPSpawnVolume::SpawnMonster()
{
	if(!bActive || !SpawnStep) return;

	PreviousSpawnDataID = SpawnStep->SpawnStepID;
	SpawnStepType = SpawnStep->SpawnStepType;

	switch (SpawnStepType)
	{
	case ESpawnStepType::Basic:
		//ActivateBattleBasicUI();
		break;
	
	case ESpawnStepType::Timer:
		ActivateBattleTimerUI();
		break;
	}

	RespawnMonster();
}

void AAPSpawnVolume::RespawnMonster() // 리스폰
{
	if(!bActive || !SpawnStep) return;

	for(auto SpawnEntry : SpawnStep->SpawnEntry)
	{
		TArray<AEnemy_CharacterBase*> CurrentEnemy;

		if (SpawnEntry.SpawnEntity->IsChildOf(AEnemy_BossBase::StaticClass()))
		{
			CurrentEnemy = SpawnMonsterComp->SpawnMonsterRandomWithLocation(SpawnEntry.SpawnEntity, SpawnEntry.EntityQuantity, GetActorLocation());
		}
		else
		{
			CurrentEnemy = SpawnMonsterComp->SpawnMonsterRandomWithSquare(SpawnEntry.SpawnEntity, SpawnEntry.EntityQuantity, AreaPoint_1, AreaPoint_2, AreaPoint_3, AreaPoint_4);		
		}
		
		Enemies.Append(CurrentEnemy);
	}
}

void AAPSpawnVolume::DespawnMonster()
{
	for(auto Enemy : Enemies)
	{
		Enemy->Destroy();
	}

	Enemies.Empty();

	CheckNextSpawn();
}

void AAPSpawnVolume::MonsterKilled(AEnemy_CharacterBase *Enemy)
{
	Enemies.Remove(Enemy);

	if(Enemies.IsEmpty())
	{
		switch (SpawnStepType)
		{
		case ESpawnStepType::Basic:
			CheckNextSpawn();
			break;
		
		case ESpawnStepType::Timer:
			RespawnMonster();
			break;
		}
	}
}

void AAPSpawnVolume::CheckNextSpawn()
{
	if(!SpawnData) return;
	
	if(PreviousFinishSteps.Contains(PreviousSpawnDataID))
	{
		SpawnStep = PreviousFinishSteps[PreviousSpawnDataID];
		SpawnMonster();
	}
}
