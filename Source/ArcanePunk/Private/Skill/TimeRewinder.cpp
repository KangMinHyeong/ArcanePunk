
#include "Skill/TimeRewinder.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/Common/APGhostTrailSpawnComponent.h"

ATimeRewinder::ATimeRewinder()
{
    RewinderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RewinderSphere"));
    RewinderEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RewinderEffect"));

    SetRootComponent(RewinderSphere);
    RewinderEffect->SetupAttachment(RewinderSphere);
}

void ATimeRewinder::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void ATimeRewinder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(bRewind && OwnerCharacter.IsValid())
    {
        FVector Current = OwnerCharacter->GetActorLocation();
        Current = FMath::VInterpConstantTo(Current, RewinderLocation, DeltaTime, RewindSpeed);
        OwnerCharacter->SetActorLocation(Current);
        
        if(abs(Current.X - RewinderLocation.X) < 0.1f && abs(Current.Y - RewinderLocation.Y) < 0.1f)
        {
            OwnerCharacter->GetGhostTrailSpawnComp()->SetSkillTrail(false);
            bRewind = false;
            SetActorTickEnabled(false);
            SetTimeRewinderAttack();
        }
    }
}

void ATimeRewinder::DestroySKill()
{
    Super::DestroySKill();
}

void ATimeRewinder::SetTimeRewinderAttack()
{
    if(!OwnerCharacter.IsValid()) return; 
    
    OwnerCharacter->GetAttackComponent()->MultiAttack(OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorUpVector() * 25.0f, RewinderWidth, DamageCoefficient, HitNumbers, bStun, StateTime);
    // Effect 추가
    // OwnerCharacter->SetActorHiddenInGame(false);
    // RewinderEffect->Activate();
    OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    DeActivate_Ult();
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ATimeRewinder::DestroySKill, 3.0f, false);
}

void ATimeRewinder::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return; 
    
    OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    // OwnerCharacter->SetActorHiddenInGame(true);
    OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RewinderLocation.Z = RewinderLocation.Z + OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

    float RewinderSize = (RewinderWidth / RewinderSphere->GetScaledSphereRadius());
    SetActorScale3D(GetActorScale3D() * (RewinderWidth / RewinderSize )); 
    SetActorLocation(RewinderLocation);

    RewinderEffect->SetNiagaraVariableFloat(TEXT("Size"), RewinderSize * 0.1f);
    RewinderEffect->SetNiagaraVariableFloat(TEXT("LoopDelay"),  (OwnerCharacter->GetActorLocation() - RewinderLocation).Size() / RewindSpeed);

    // 투명화 추가?
    bRewind = true; SetActorTickEnabled(true);
}

