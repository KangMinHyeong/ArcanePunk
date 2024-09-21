
#include "GameMode/APGameModeBattleStage.h"

#include "GameElements/Trigger/BattleSection/APBattleSectionBase.h"
#include "EngineUtils.h"
#include "ArcanePunk/Public/Enemy/Enemy_CharacterBase.h"
#include "GameState/APGameState.h"
#include "GameElements/Portal/Portal_Base.h"
#include "GameFramework/Controller.h"
#include "AIController.h"
#include "GameElements/Trap/APTrapBase.h"
#include "Enemy/Drop/Enemy_DropPackage.h"

void AAPGameModeBattleStage::MonsterKilled(AActor* BattleSection)
{
    for(AEnemy_CharacterBase* Enemy : TActorRange<AEnemy_CharacterBase>(GetWorld()))
    {
       if(!Enemy->IsDead()) return;
    }   

    CheckBattleSection(BattleSection);	
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

void AAPGameModeBattleStage::CheckBattleSection(AActor* BattleSection)
{
    auto BS = Cast<AAPBattleSectionBase>(BattleSection); if(!BS) return;

    // 몬스터가 다 처지 되었으면 호출, 배틀섹션에서 추가로 몬스터를 스폰할껀지 확인, 확인해서 배틀섹션 종료 및 지속 결정
	if(BS->CheckSpawnEnd())
    {
        for(auto Trap : TActorRange<AAPTrapBase>(GetWorld()))
            Trap->Deactivate();

        GetWorld()->GetWorldSettings()->SetTimeDilation(0.25f);
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AAPGameModeBattleStage::PortalSpawn, BS->GetBattleSectionID());
	    GetWorld()->GetTimerManager().SetTimer(PortalSpawnTimerHandle, TimerDelegate, 1.0f, false);
    }
}

void AAPGameModeBattleStage::PortalSpawn(FName CurrentClearStage)
{
    GetWorld()->GetTimerManager().ClearTimer(PortalSpawnTimerHandle);
    GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
    for(auto Portal : TActorRange<APortal_Base>(GetWorld()))
    {
        if(CurrentClearStage == Portal->GetPortalID())
        {
            Portal->InitHide(false);
        }
    }

    for(auto Drop : TActorRange<AEnemy_DropPackage>(GetWorld()))
    {
        if(CurrentClearStage == Drop->GetDropID())
            Drop->DropActivate();
    }
}

void AAPGameModeBattleStage::StartPlay()
{
	Super::StartPlay();
	//GetWorldTimerManager().SetTimer(MonsterSpawnTimerHandle, this, &AAPGameModeBase::SpawnMonster, SpawnInterval, true);
}