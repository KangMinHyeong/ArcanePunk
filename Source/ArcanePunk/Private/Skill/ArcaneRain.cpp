
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

    SetRootComponent(RainRoot);

    RainRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    SkillCategory = ESkillCategory::InvisibleCollider;
}

void AArcaneRain::BeginPlay()
{
    Super::BeginPlay();
}

void AArcaneRain::SetRainEffect()
{
    // float Size =  GetActorScale3D().Y / DefaultSize;
    // auto Character = Cast<AArcanePunkCharacter>(GetOwner()); if(!Character) return;
    // if(RainEffect) RainComp = UNiagaraFunctionLibrary::SpawnSystemAttached(RainEffect, Character->GetMesh(), TEXT("Shouting"), FVector(0,0,0), FRotator::ZeroRotator, Size * ShoutScale,EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true);
    // if(!RainComp) return; 
    // RainComp->SetNiagaraVariableLinearColor(TEXT("Color"),  EffectColor);
    SetRainAttack();
}

void AArcaneRain::SetRainAttack()
{
    float Size =  GetActorScale3D().Y / DefaultSize;
    auto Character = Cast<AArcanePunkCharacter>(GetOwner()); if(!Character) return;
    Character->GetAttackComponent()->MultiAttack(Character->GetActorLocation(), Character->GetActorLocation() + Character->GetActorUpVector() * 25.0f, RainRadius * Size, DamageCoefficient, HitNumbers, bStun, StateTime);
    DrawDebugSphere(GetWorld(), Character->GetMesh()->GetComponentLocation(), RainRadius* Size, 18, FColor::Green,false, 2.5f);
}

void AArcaneRain::DestroySKill()
{
    Super::DestroySKill();
}

void AArcaneRain::SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility)
{
    Super::SetSkill(SkillType, SkillAbility);
}
