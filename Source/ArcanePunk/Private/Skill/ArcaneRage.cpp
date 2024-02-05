// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/ArcaneRage.h"

#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/Character/APSkillHubComponent.h"

AArcaneRage::AArcaneRage()
{
    RageRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("RageRoot"));
    RageEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RageEffect"));

    SetRootComponent(RageRoot);
    RageEffect->SetupAttachment(RageRoot);

    RageRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    SkillCategory = ESkillCategory::Buff;
}

void AArcaneRage::BeginPlay()
{
    Super::BeginPlay();
}

void AArcaneRage::SetRageMode()
{
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter) return;
    OwnerCharacter->SetRageMode(true);
}

void AArcaneRage::DestroySKill()
{
    Super::DestroySKill();

    if(OwnerCharacter) OwnerCharacter->SetRageMode(false);
    if(OwnerCharacter) OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber12()->bActivate = false;
}

void AArcaneRage::SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility)
{
    Super::SetSkill(SkillType, SkillAbility);

    SetRageMode();
}
