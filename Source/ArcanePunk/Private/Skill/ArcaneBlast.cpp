
#include "Skill/ArcaneBlast.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Enemy/Enemy_CharacterBase.h"

AArcaneBlast::AArcaneBlast()
{
    BlastTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("BlastTrigger"));
    BlastEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BlastEffectComp"));

    SetRootComponent(BlastTrigger);
    BlastEffectComp->SetupAttachment(BlastTrigger);

    BlastMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BlastMoveComp"));

}

void AArcaneBlast::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);   
}

void AArcaneBlast::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AArcaneBlast::DestroySKill()
{
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);  

    BlastTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    BlastMoveComp->MaxSpeed = 0.01f;
	BlastMoveComp->InitialSpeed = 0.01f;
    if(BlastEffectComp) BlastEffectComp->Deactivate();

    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArcaneBlast::DestroySKill_Immediate, DestroyDelayTime, false);
}

void AArcaneBlast::DestroySKill_Immediate()
{
    Destroy();
}

void AArcaneBlast::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

    if(OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
       	if(bStun) HitPointComp->SetCrowdControl(OtherActor, ECharacterState::Stun, StateTime);
		HitPointComp->DistinctHitPoint(BlastHitEffect, Hit.ImpactPoint, OtherActor);
        OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); 
        if(OwnerCharacter.IsValid()) 
        {
            float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, Hit, true);
		    OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
        }
	}
    DestroySKill();
}


void AArcaneBlast::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;

    BlastTrigger->OnComponentHit.AddDynamic(this, &AArcaneBlast::OnHitting);
    BlastTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
    BlastTrigger->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
    BlastTrigger->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Block); 
    BlastTrigger->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Block);
    BlastTrigger->SetCollisionResponseToChannel(ECC_Vehicle, ECollisionResponse::ECR_Block);
    BlastTrigger->SetCollisionResponseToChannel(ECC_Destructible, ECollisionResponse::ECR_Block);

    BlastMoveComp->MaxSpeed = BlastSpeed;
	BlastMoveComp->InitialSpeed = BlastSpeed;
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArcaneBlast::DestroySKill, DestroyTime, false);
}

