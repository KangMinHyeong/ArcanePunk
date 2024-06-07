
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

void AArcaneRain::SetRainAttack(FVector CollisionLocation)
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

void AArcaneRain::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;
    RainComp->SetNiagaraVariableFloat(TEXT("Size"), RainSize);
    RainComp->SetNiagaraVariableFloat(TEXT("RainSpawnRate"), RainSpawnRate);

}

