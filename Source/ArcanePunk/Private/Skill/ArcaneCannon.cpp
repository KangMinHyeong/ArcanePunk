
#include "Skill/ArcaneCannon.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Enemy/Enemy_CharacterBase.h"

AArcaneCannon::AArcaneCannon()
{
    CannonTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("CannonTrigger"));
    CannonEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CannonEffect"));

    SetRootComponent(CannonTrigger);
    CannonEffect->SetupAttachment(CannonTrigger);

    CannonMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("CannonMoveComp"));
}

void AArcaneCannon::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);   
}

void AArcaneCannon::DestroySKill()
{
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);  

    CannonTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CannonMoveComp->MaxSpeed = 0.01f;
	CannonMoveComp->InitialSpeed = 0.01f;
    CannonEffect->Deactivate();

    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArcaneCannon::DestroySKill_Immediate, DestroyTime, false);
}

void AArcaneCannon::DestroySKill_Immediate()
{
    Destroy();
}

void AArcaneCannon::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

    if(OtherActor && OtherActor != this && OtherActor != MyOwner && OwnerCharacter.IsValid())
	{
        OwnerCharacter->GetAttackComponent()->MultiAttack(GetActorLocation(), GetActorLocation(), CannonWidth * ExplosionRange, DamageCoefficient, 1, false, StateTime );
	}

    auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), CannonHitEffect, GetActorLocation(), GetActorRotation());
    NC->SetNiagaraVariableFloat(TEXT("Size"), (CannonWidth * ExplosionRange) / DefaultCannonWidth);

    DestroySKill();
}

void AArcaneCannon::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;
    
    CannonTrigger->SetSphereRadius(CannonWidth);

    CannonTrigger->OnComponentHit.AddDynamic(this, &AArcaneCannon::OnHitting);
    CannonTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
    CannonTrigger->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
    CannonTrigger->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Block); 
    CannonTrigger->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Block);
    CannonTrigger->SetCollisionResponseToChannel(ECC_Vehicle, ECollisionResponse::ECR_Block);
    CannonTrigger->SetCollisionResponseToChannel(ECC_Destructible, ECollisionResponse::ECR_Block);

    CannonMoveComp->MaxSpeed = CannonSpeed;
	CannonMoveComp->InitialSpeed = CannonSpeed;

    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArcaneCannon::DestroySKill, MaxCannonDist / CannonSpeed, false);
}


