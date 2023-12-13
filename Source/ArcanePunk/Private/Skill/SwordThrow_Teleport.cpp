
#include "Skill/SwordThrow_Teleport.h"

#include "Enemy/Enemy_CharacterBase.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSkillNumber.h"
#include "Components/Character/SkillNumber/SkillNumber2.h"
#include "Components/Character/APSkillHubComponent.h"


void ASwordThrow_Teleport::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    Super::OnHitting(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

    SetActorHiddenInGame(true);
    ActiveMark(OtherActor);
}

void ASwordThrow_Teleport::ActiveMark(AActor *OtherActor)
{
    auto Enemy = Cast<AEnemy_CharacterBase>(OtherActor);
    if(!Enemy) {DestroySword(); return;}
    auto Character = Cast<AArcanePunkCharacter>(GetOwner());
    if(!Character) return;

    FVector EnemyLocation = Enemy->GetActorLocation() + Enemy->GetActorForwardVector() * MarkDistance;

    Enemy->TeleportMarkActivate(DestroyTime);
    Character->GetAPSkillNumberComponent()->GetSkillNumber2()->MarkingOn(EnemyLocation, OtherActor, DestroyTime);
}

void ASwordThrow_Teleport::DestroySword()
{
    auto Character = Cast<AArcanePunkCharacter>(GetOwner());
    if(!Character) return;
    Character->GetAPSkillHubComponent()->RemoveSkillState();

    Destroy();
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}
