
#include "Skill/SwordThrow_Teleport.h"

#include "Enemy/Enemy_CharacterBase.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/BoxComponent.h"

ASwordThrow_Teleport::ASwordThrow_Teleport()
{
}

void ASwordThrow_Teleport::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    Super::OnHitting(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

    SetActorHiddenInGame(true);
    HitTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ActiveMark(OtherActor);
}

void ASwordThrow_Teleport::ActiveMark(AActor *OtherActor)
{
    auto Enemy = Cast<AEnemy_CharacterBase>(OtherActor);
    if(!Enemy) {DestroySKill(); return;}
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
    if(!OwnerCharacter.IsValid()) return;

    Enemy->TeleportMarkActivate(DestroyTime, GetOwner(), SkillComp.Get());

    // TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_2);
	if(SkillComp.IsValid())
	{
		SkillComp->MarkingOn(OtherActor, DestroyTime);
	} 
}

void ASwordThrow_Teleport::DestroySKill()
{
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
    if(!OwnerCharacter.IsValid()) return;
    // Character->GetAPSkillHubComponent()->RemoveSkillState();

    Destroy();
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

