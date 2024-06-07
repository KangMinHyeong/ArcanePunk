
#include "Skill/ArcaneReflector.h"

#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Enemy/Enemy_CharacterBase.h"

AArcaneReflector::AArcaneReflector()
{
    DotTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("DotTrigger"));
    DotRangeEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DotRangeEffect"));

    SetRootComponent(DotTrigger);
    DotRangeEffect->SetupAttachment(DotTrigger);
}

void AArcaneReflector::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);  
}

void AArcaneReflector::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(bCharging)
    {
        OnReflectingCheck(DeltaTime);
    }
}

void AArcaneReflector::DestroySKill()
{
    if(!OwnerCharacter.IsValid()) return;
    if(OwnerCharacter->GetReflectMode() > 0) OwnerCharacter->SetReflectMode(OwnerCharacter->GetReflectMode() - 1);
    DeActivate_Ult();
    ReflectShield->Destroy();
    Super::DestroySKill();
}

void AArcaneReflector::OnReflectingCheck(float DeltaTime)
{
    if(!OwnerCharacter.IsValid()) return;
    if(OwnerCharacter->GetOnRSkill())
    {
        OwnerCharacter->GetAPHUD()->OnChargingGauge.Broadcast( CurrentReflectTime - ChargeCurrent , ChargeMax );
        CurrentReflectTime = FMath::FInterpConstantTo(CurrentReflectTime, ReflectTime, DeltaTime, ChargeSpeed);
    }
    else
    {
        StartReflectingMode();
    }
}

void AArcaneReflector::StartReflectingMode()
{
    OwnerCharacter->GetAPHUD()->OnChargingEnd.Broadcast(false);
    GetWorldTimerManager().ClearTimer(ReflectTimerHandle);
    if(!OwnerCharacter.IsValid()) return;
    bCharging = false;
    SetActorTickEnabled(false);
    auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
    if(OwnerAnim) OwnerAnim->StopUltSkill_12_Montage();
    DotDamage( CurrentReflectTime );
    OnReflectingMode();
}

void AArcaneReflector::OnReflectingMode()
{
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArcaneReflector::DestroySKill, CurrentReflectTime, false);

    DotTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
    DotRangeEffect->DeactivateImmediate();

    UNiagaraFunctionLibrary::SpawnSystemAttached(ReflectorEffect, OwnerCharacter->GetRootComponent(), TEXT("ReflectorEffect"), FVector(0,0,0), FRotator::ZeroRotator, FVector(1,1,1),EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true);
    OwnerCharacter->GetRSkillNumber()->Activate_Skill();

    OwnerCharacter->SetReflectMode(OwnerCharacter->GetReflectMode() + 1);
    SpawnReflector();
}

void AArcaneReflector::SpawnReflector()
{
    ReflectShield = GetWorld()->SpawnActor<AActor>(ReflectShieldClass, OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
    if(!ReflectShield.IsValid()) return;
    ReflectShield->SetOwner(OwnerCharacter.Get());
    ReflectShield->SetActorLocation(ReflectShield->GetActorLocation() + OwnerCharacter->GetActorUpVector() * 100.0f);
    ReflectShield->AttachToComponent(OwnerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

    auto RootComp = ReflectShield->GetComponentByClass<UStaticMeshComponent>();
    RootComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
    RootComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Block); 
    RootComp->SetCollisionResponseToChannel(ECC_Vehicle, ECollisionResponse::ECR_Block);
    RootComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Block);
    RootComp->SetCollisionResponseToChannel(ECC_Destructible, ECollisionResponse::ECR_Block);
}

void AArcaneReflector::DotDamage(float Multiple)
{
    FHitResult HitResult;
    TArray<AActor*> Actors;
    DotTrigger->GetOverlappingActors(Actors, AEnemy_CharacterBase::StaticClass());
    for(auto Enemy : Actors)
    {
        float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(Enemy, OwnerCharacter->GetCurrentATK() * DamageCoefficient * Multiple, HitResult, false);
    }
}

void AArcaneReflector::SetDotRange(float Radius)
{
    DotTriggerRange = DotTrigger->GetScaledSphereRadius();
    DotTriggerRangeSize = Radius / DotTriggerRange;
}

void AArcaneReflector::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;

    DotTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    DotTrigger->SetSphereRadius(DotTriggerRangeSize * DotTriggerRange);
    DotRangeEffect->SetNiagaraVariableFloat(TEXT("Size"), DotTriggerRangeSize);
    DotRangeEffect->Activate();

    ChargeSpeed = (ReflectTime - CurrentReflectTime) / ReflectTime;

    FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AArcaneReflector::DotDamage, 1.0f);
    GetWorldTimerManager().SetTimer(DamageHandle, TimerDelegate, DotRate, true);
    GetWorldTimerManager().SetTimer(ReflectTimerHandle, this, &AArcaneReflector::StartReflectingMode, MaxflectTime, false);

    ChargeMax = (ReflectTime - CurrentReflectTime);
    ChargeCurrent = CurrentReflectTime;
}
