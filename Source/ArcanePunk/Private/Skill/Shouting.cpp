
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

    // if(ShoutComp.IsValid()) {ShoutComp->DestroyComponent();}
}

void AShouting::SetShoutingEffect()
{
    // float Size =  GetActorScale3D().Y / DefaultSize;
    // auto Character = Cast<AArcanePunkCharacter>(GetOwner()); if(!Character) return;
    // if(ShoutEffect) ShoutComp = UNiagaraFunctionLibrary::SpawnSystemAttached(ShoutEffect, Character->GetMesh(), TEXT("Shouting"), FVector(0,0,0), FRotator::ZeroRotator, Size * ShoutScale,EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true);
    // if(!ShoutComp.IsValid()) return; 
    // ShoutComp->SetNiagaraVariableLinearColor(TEXT("Color"),  EffectColor);
    SetShoutingAttack();
}

void AShouting::SetShoutingAttack()
{
    float Size =  GetActorScale3D().Y / DefaultSize;
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->GetAttackComponent()->MultiAttack(OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorUpVector() * 25.0f, ShoutRadius * Size, DamageCoefficient, 1, true, StateTime);
    DrawDebugSphere(GetWorld(), OwnerCharacter->GetMesh()->GetComponentLocation(), ShoutRadius* Size, 18, FColor::Green,false, 2.5f);
}

void AShouting::SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility)
{
    Super::SetSkill(SkillType, SkillAbility);
}

