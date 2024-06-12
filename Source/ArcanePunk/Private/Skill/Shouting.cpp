
#include "Skill/Shouting.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "DrawDebugHelpers.h"

AShouting::AShouting()
{
    ShoutingRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("ShoutingRoot"));

    SetRootComponent(ShoutingRoot);

    ShoutingRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    SkillCategory = ESkillCategory::InvisibleCollider;
}

void AShouting::BeginPlay()
{
    Super::BeginPlay();
    InitShoutRadius = ShoutRadius;
}

void AShouting::DestroySKill()
{
    Super::DestroySKill();
}

void AShouting::SetShoutingEffect()
{
    if(!OwnerCharacter.IsValid()) return;
    if(ShoutEffect) ShoutComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShoutEffect, GetActorLocation() + GetActorUpVector()*50.0f, GetActorRotation());
    if(!ShoutComp.IsValid()) return; 
    ShoutComp->SetNiagaraVariableFloat(TEXT("Size"), ShoutRadius / InitShoutRadius);
    
}

void AShouting::SetShoutingAttack()
{
    if(!OwnerCharacter.IsValid()) return;
    float LionHowlingCoeff = 1.0f;
    if(LionHowling) LionHowlingCoeff = 2.0f;

    OwnerCharacter->GetAttackComponent()->MultiAttack(OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorUpVector() * 25.0f, ShoutRadius, DamageCoefficient, 1, true, StateTime * LionHowlingCoeff);
    DestroySKill();
}

void AShouting::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);

    for(auto It : SkillAbilityNestingData.SilverAbilityNestingNum)
    {
        CheckSilverEnhance(It.Key, It.Value);
    }
    for(auto It : SkillAbilityNestingData.GoldAbilityNestingNum)
    {
        CheckGoldEnhance(It.Key, It.Value);
    }
    for(auto It : SkillAbilityNestingData.PlatinumAbilityNestingNum)
    {
        CheckPlatinumEnhance(It.Key, It.Value);
    }

    SetShoutingEffect();
    SetShoutingAttack();
}

void AShouting::CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum)
{
	Super::CheckSilverEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // Damage Up
        SkillAbilityComponent->Coefficient_Add(DamageCoefficient,AbilityData->Coefficient_X, NestingNum);
        break;

		case 2: // 범위 증가
		SkillAbilityComponent->Coefficient_AddMultiple(ShoutRadius, AbilityData->Coefficient_X, NestingNum);
        break;
    }
}

void AShouting::CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckGoldEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // Damage Up
        SkillAbilityComponent->Coefficient_Add(DamageCoefficient,AbilityData->Coefficient_X, NestingNum);
        break;

        case 2: // 기절 시간 증가
		SkillAbilityComponent->Coefficient_Add(StateTime, AbilityData->Coefficient_X, NestingNum);
        break;
    }
}

void AShouting::CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckPlatinumEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // Damage Up
        SkillAbilityComponent->Coefficient_Add(DamageCoefficient,AbilityData->Coefficient_X, NestingNum);
        break;
    }
}

