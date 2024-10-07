
#include "Skill/ArcaneRain.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "DrawDebugHelpers.h"

AArcaneRain::AArcaneRain()
{
    RainRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("RainRoot"));
    RainComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RainComp"));
    SetRootComponent(RainRoot);
    RainComp->SetupAttachment(RainRoot);

    RainRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    SkillCategory = ESkillCategory::InvisibleCollider;
}

void AArcaneRain::BeginPlay()
{
    Super::BeginPlay();
}

void AArcaneRain::SetRainAttack(const FVector & CollisionLocation)
{
    if(!OwnerCharacter.IsValid()) return;
    // float Size =  GetActorScale3D().Y / DefaultSize;

    OwnerCharacter->GetAttackComponent()->MultiAttack(CollisionLocation, CollisionLocation, RainRadius, DamageCoefficient, HitNumbers, bStun, StateTime);
    // DrawDebugSphere(GetWorld(), Character->GetMesh()->GetComponentLocation(), RainRadius* Size, 18, FColor::Green,false, 2.5f);
}

void AArcaneRain::DestroySKill()
{
    Super::DestroySKill();
}

void AArcaneRain::SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;

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

    RainComp->SetVariableFloat(TEXT("Size"), RainSize);
    RainComp->SetVariableFloat(TEXT("RainSpawnRate"), RainSpawnRate);

}

void AArcaneRain::CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum)
{
	Super::CheckSilverEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // Damage Up
        SkillAbilityComponent->Coefficient_Add(DamageCoefficient,AbilityData->Coefficient_X, NestingNum);
        break;

        case 2: // Range Up
        SkillAbilityComponent->Coefficient_AddMultiple(RainSize,AbilityData->Coefficient_X, NestingNum);
        break;
    }
}

void AArcaneRain::CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckGoldEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // Damage Up
        SkillAbilityComponent->Coefficient_Add(DamageCoefficient, AbilityData->Coefficient_X, NestingNum);
        break;

        case 2: // RainSpawnRate Up
        SkillAbilityComponent->Coefficient_Add(RainSpawnRate, AbilityData->Coefficient_X, NestingNum);
        break;

    }
}

void AArcaneRain::CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckPlatinumEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // Damage Up
        SkillAbilityComponent->Coefficient_Add(DamageCoefficient,AbilityData->Coefficient_X, NestingNum);
        break;
    }
}

