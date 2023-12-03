// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SwordThrow_Teleport.h"

#include "Enemy/Enemy_CharacterBase.h"
#include "Character/ArcanePunkCharacter.h"

void ASwordThrow_Teleport::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    Super::OnHitting(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

    ActiveMark(OtherActor);
    Destroy();
}

void ASwordThrow_Teleport::ActiveMark(AActor *OtherActor)
{
    auto Enemy = Cast<AEnemy_CharacterBase>(OtherActor);
    if(!Enemy) return;
    auto Character = Cast<AArcanePunkCharacter>(GetOwner());
    if(!Character) return;

    FVector EnemyLocation = Enemy->GetActorLocation() + Enemy->GetActorForwardVector() * MarkDistance;

    Enemy->TeleportMarkActivate(Character->SwordTeleportTime);
    Character->MarkingOn(EnemyLocation, OtherActor);
}
