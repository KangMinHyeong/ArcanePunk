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
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->SetRageMode(true);
}

void AArcaneRage::DestroySKill()
{
    if(OwnerCharacter.IsValid()) OwnerCharacter->SetRageMode(false);

    DeActivate(ESkillNumber::Skill_12);

    Super::DestroySKill();
}

void AArcaneRage::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData)
{
    Super::SetSkill(SkillAbilityNestingData);

    SetRageMode();
}
