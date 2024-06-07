
#include "Skill/BomberMan.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/Enemy_CharacterBase.h"

ABomberMan::ABomberMan()
{
    BomberTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("BomberTrigger"));

    SetRootComponent(BomberTrigger);
}

void ABomberMan::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void ABomberMan::FastBuff()
{
    OwnerCharacter->GetCrowdControlComp()->FastState(FastCoefficient, DestroyTime);
}

void ABomberMan::DestroySKill()
{
    DeActivate_Ult();
    Super::DestroySKill();
}

void ABomberMan::OnExplosion()
{
    auto NC = UNiagaraFunctionLibrary::SpawnSystemAttached(BomberEffect, GetRootComponent(), TEXT("BomberEffect"), FVector(0,0,0), FRotator::ZeroRotator, FVector(1,1,1),EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true);
    // SpawnSystemAttached(BomberEffect, OwnerCharacter->GetRootComponent(), TEXT("BomberEffect"), FVector(0,0,0), FRotator::ZeroRotator, FVector(1,1,1),EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true);
    NC->SetNiagaraVariableFloat(TEXT("Time"), BomberHitRate);
    NC->SetNiagaraVariableFloat(TEXT("Size"), BomberTrigger->GetScaledSphereRadius() * 0.01f);

    TArray<AActor*> Actors;
    BomberTrigger->GetOverlappingActors(Actors, AEnemy_CharacterBase::StaticClass());
    for(auto Enemy : Actors)
    {
        HitDelay(Enemy, OwnerCharacter->GetCurrentATK() * DamageCoefficient, 1, HitDelayTime, true);
    }
}

void ABomberMan::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;

    BomberTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    BomberTrigger->SetSphereRadius(OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius() * BomberSize);
    
    FastBuff(); OnExplosion();

    GetWorldTimerManager().SetTimer(BomberTimerHandle, this, &ABomberMan::OnExplosion, BomberHitRate, true);
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ABomberMan::DestroySKill, DestroyTime, false);
}