
#include "GameMode/APGameModeBattleStage.h"

#include "BattleSection/APBattleSectionBase.h"
#include "EngineUtils.h"
#include "ArcanePunk/Public/Enemy/Enemy_CharacterBase.h"
#include "GameState/APGameState.h"
#include "Portal/Portal_Base.h"
#include "GameFramework/Controller.h"
#include "AIController.h"

void AAPGameModeBattleStage::MonsterKilled()
{
    if(RemainMonsterNumber > 0) RemainMonsterNumber--;
    for(AEnemy_CharacterBase* Enemy : TActorRange<AEnemy_CharacterBase>(GetWorld()))
    {
       if(!Enemy->IsDead()) return;
    }   

    EndBattleSection();	
}

void AAPGameModeBattleStage::PlayerKilled()
{
    for(AController* Controller : TActorRange<AController>(GetWorld()))
    {
        auto AIC = Cast<AAIController>(Controller); if(!AIC) continue;
        AIC->GameHasEnded(Controller->GetPawn(), false);
        AIC->UnPossess();
    } 
}

void AAPGameModeBattleStage::EndBattleSection()
{
    // 몬스터가 다 처지 되었으면 호출, 배틀섹션에서 추가로 몬스터를 스폰할껀지 확인, 확인해서 배틀섹션 종료 및 지속 결정
	for(auto BattleSection : TActorRange<AAPBattleSectionBase>(GetWorld()))
    {
        if(BattleSection->CheckSpawnEnd())
        {
            auto GS = Cast<AAPGameState>(GetWorld()->GetGameState()); if(!GS) return;
            GS->SubStageClearMap.Add(BattleSection->GetBattleSectionID(), true);
            CurrentClearStage = BattleSection->GetBattleSectionID();
            GetWorldTimerManager().SetTimer(PortalSpawnTimerHandle, this, &AAPGameModeBattleStage::PortalSpawn, 2.5f, false);
        }
    }
}

void AAPGameModeBattleStage::PortalSpawn()
{
    for(auto Portal : TActorRange<APortal_Base>(GetWorld()))
    {
        if(CurrentClearStage == Portal->GetPortalID())
        {
            Portal->InitHide(false);
        }
    }
}

void AAPGameModeBattleStage::StartPlay()
{
	Super::StartPlay();
	//GetWorldTimerManager().SetTimer(MonsterSpawnTimerHandle, this, &AAPGameModeBase::SpawnMonster, SpawnInterval, true);
}