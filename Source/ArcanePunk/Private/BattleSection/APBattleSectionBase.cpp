
#include "BattleSection/APBattleSectionBase.h"

#include "Components/BoxComponent.h"
#include "Components/Common/APSpawnMonsterComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "SpawnPoint/APSpawnPointBase.h"
#include "GameState/APGameState.h"

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

void AAPBattleSectionBase::ClearChecking() // 인스턴스 던전에서 해당 배틀 섹션을 클리어 하였는지 체크
{
	auto GS = Cast<AAPGameState>(GetWorld()->GetGameState()); if(!GS) return;

	if(GS->SubStageClearMap.Contains(StageNumber)) {Destroy();}
	else {BattleTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAPBattleSectionBase::OnSpawnLocation);}
}

void AAPBattleSectionBase::OnSpawnLocation(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	BattleTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(SpawnPointClasses.IsEmpty()) return;
	SpawnMonsterComp->SpawnLocationRandom(SpawnPointClasses[0], SpawnNumber, SpawnLocation, SpawnRoot->GetComponentLocation(), SpawnRoot2->GetComponentLocation());
	GetWorldTimerManager().SetTimer(SpawnMonsterTimerHandle, this, &AAPBattleSectionBase::SpawnMonster, SpawnMonsterTime, false);
}

void AAPBattleSectionBase::SpawnMonster()
{
	if(SpawnMonsterClasses.IsEmpty()) return;
	SpawnMonsterComp->SpawnMonsterFromLocation(SpawnMonsterClasses[0], SpawnLocation, MonsterArr);

	if(TotalSpawnNumber > 0) TotalSpawnNumber--;
	GetWorldTimerManager().ClearTimer(SpawnMonsterTimerHandle);
}
