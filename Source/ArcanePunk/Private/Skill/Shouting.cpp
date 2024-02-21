
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
}

void AShouting::DestroySKill()
{
    Super::DestroySKill();

    // if(ShoutComp) {ShoutComp->DestroyComponent();}
}

void AShouting::SetShoutingEffect()
{
    // float Size =  GetActorScale3D().Y / DefaultSize;
    // auto Character = Cast<AArcanePunkCharacter>(GetOwner()); if(!Character) return;
    // if(ShoutEffect) ShoutComp = UNiagaraFunctionLibrary::SpawnSystemAttached(ShoutEffect, Character->GetMesh(), TEXT("Shouting"), FVector(0,0,0), FRotator::ZeroRotator, Size * ShoutScale,EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true);
    // if(!ShoutComp) return; 
    // ShoutComp->SetNiagaraVariableLinearColor(TEXT("Color"),  EffectColor);
    SetShoutingAttack();
}

void AShouting::SetShoutingAttack()
{
    float Size =  GetActorScale3D().Y / DefaultSize;
    auto Character = Cast<AArcanePunkCharacter>(GetOwner()); if(!Character) return;
    Character->GetAttackComponent()->MultiAttack(Character->GetActorLocation(), Character->GetActorLocation() + Character->GetActorUpVector() * 25.0f, ShoutRadius * Size, DamageCoefficient, 1, true, StateTime);
    DrawDebugSphere(GetWorld(), Character->GetMesh()->GetComponentLocation(), ShoutRadius* Size, 18, FColor::Green,false, 2.5f);
}

void AShouting::SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility)
{
    Super::SetSkill(SkillType, SkillAbility);
}

