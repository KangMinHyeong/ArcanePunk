
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
    float Size =  GetActorScale3D().Y / DefaultSize;
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;

    SpawnRainEffect(OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorUpVector()*100.0f, OwnerCharacter->GetActorRotation());
    // if(RainEffect) {RainComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), RainEffect, Character->GetActorLocation() + Character->GetActorUpVector()*100.0f, Character->GetActorRotation(), RainScale*Size);}
    // if(!RainComp.IsValid()) return; 
    // RainComp->SetNiagaraVariableObject(TEXT("BPCallback"), this);
    
    SetRainAttack();
}

void AArcaneRain::SetRainAttack()
{
    if(!OwnerCharacter.IsValid()) return;
    float Size =  GetActorScale3D().Y / DefaultSize;

    OwnerCharacter->GetAttackComponent()->MultiAttack(OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorUpVector() * 25.0f, RainRadius * Size, DamageCoefficient, HitNumbers, bStun, StateTime);
    // DrawDebugSphere(GetWorld(), Character->GetMesh()->GetComponentLocation(), RainRadius* Size, 18, FColor::Green,false, 2.5f);
}

void AArcaneRain::DestroySKill()
{
    Super::DestroySKill();
}

void AArcaneRain::ReceiveParticleData(const TArray<FBasicParticleData> &Data, UNiagaraSystem *NiagaraSystem, const FVector &SimulationPositionOffset)
{
    UE_LOG(LogTemp, Display, TEXT("Your message"));
}

void AArcaneRain::SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility)
{
    Super::SetSkill(SkillType, SkillAbility);
}

