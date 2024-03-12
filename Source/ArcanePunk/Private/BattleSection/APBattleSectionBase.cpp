
#include "BattleSection/APBattleSectionBase.h"

#include "Components/BoxComponent.h"
#include "Components/Common/APSpawnMonsterComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "SpawnPoint/APSpawnPointBase.h"
#include "GameState/APGameState.h"
#include "GameMode/APGameModeBattleStage.h"
#include "Kismet/GameplayStatics.h"

AAPBattleSectionBase::AAPBattleSectionBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnMonsterComp = CreateDefaultSubobject<UAPSpawnMonsterComponent>(TEXT("SpawnMonsterComp"));
    BattleTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BattleTrigger"));
    SpawnRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnRoot"));
	SpawnRoot2 = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnRoot2"));

	SetRootComponent(BattleTrigger);
	SpawnRoot->SetupAttachment(BattleTrigger);
	SpawnRoot2->SetupAttachment(BattleTrigger);
}

void AAPBattleSectionBase::BeginPlay()
{
	Super::BeginPlay();
	
	ClearChecking();
}

void AAPBattleSectionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AAPBattleSectionBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AAPBattleSectionBase::ClearChecking() // 인스턴스 던전에서 해당 배틀 섹션을 클리어 하였는지 체크 & BattleStage Information 정보들 초기화
{
	auto GS = Cast<AAPGameState>(GetWorld()->GetGameState()); if(!GS) return;

	if(GS->SubStageClearMap.Contains(BattleSectionID)) {Destroy(); return;}
	else {BattleTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAPBattleSectionBase::OnSpawnLocation);}

	auto BattleGM = Cast<AAPGameModeBattleStage>(UGameplayStatics::GetGameMode(GetWorld())); if(!BattleGM) return;
	auto DataTable = BattleGM->GetBattleStageDataTable()->FindRow<FBattleStageDataTable>(BattleSectionID, BattleSectionID.ToString());
	if(DataTable)
	{
		BattleGM->SetBattleStageInformation(DataTable->BattleStageInformation);

		uint8 TotalMonsterNum = 0;
		for(auto Data : DataTable->SpawnMonsterData)
		{
			for(auto Type : Data.SpawnMonsterType)
			{
				TotalMonsterNum = TotalMonsterNum + Type.Value;
			}
		}
		BattleGM->SetTotalMonsterNumber(TotalMonsterNum);
		BattleGM->SetRemainMonsterNumber(TotalMonsterNum);
	}
}

void AAPBattleSectionBase::OnSpawnLocation(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	BattleTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// if(SpawnPointClasses.IsEmpty()) return;
	// SpawnMonsterComp->SpawnLocationRandom(SpawnPointClasses[0], SpawnNumber, SpawnLocation, SpawnRoot->GetComponentLocation(), SpawnRoot2->GetComponentLocation());
	GetWorldTimerManager().SetTimer(SpawnMonsterTimerHandle, this, &AAPBattleSectionBase::SpawnMonster, SpawnMonsterTime, false);
}

void AAPBattleSectionBase::SpawnMonster()
{
	auto BattleGM = Cast<AAPGameModeBattleStage>(UGameplayStatics::GetGameMode(GetWorld())); if(!BattleGM) return;
	auto DataTable = BattleGM->GetBattleStageDataTable()->FindRow<FBattleStageDataTable>(BattleSectionID, BattleSectionID.ToString()); if(!DataTable)return;

	if(TotalSpawnNum < DataTable->SpawnMonsterData.Num())
	{
		for( auto Spawn : DataTable->SpawnMonsterData[TotalSpawnNum].SpawnMonsterType)
		{
			SpawnMonsterComp->SpawnMonsterRandomWithoutLocationActor(Spawn.Key, Spawn.Value, SpawnRoot->GetComponentLocation(), SpawnRoot2->GetComponentLocation());
		}
		TotalSpawnNum++;
	} 
	GetWorldTimerManager().ClearTimer(SpawnMonsterTimerHandle);
}

bool AAPBattleSectionBase::CheckSpawnEnd()
{
	auto BattleGM = Cast<AAPGameModeBattleStage>(UGameplayStatics::GetGameMode(GetWorld())); if(!BattleGM) return false;
	auto DataTable = BattleGM->GetBattleStageDataTable()->FindRow<FBattleStageDataTable>(BattleSectionID, BattleSectionID.ToString()); if(!DataTable)return false;

	if(TotalSpawnNum < DataTable->SpawnMonsterData.Num())
	{
		SpawnMonster(); // 후에 시간차로 웨이브가 생성되는 식이면 삭제, 대신 타이머 만들기
		return false;
	} 
	return true;
}
