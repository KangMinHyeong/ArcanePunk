
#include "Skill/Sub/TurretAmmo.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/Character/APHitPointComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APAttackComponent.h"

ATurretAmmo::ATurretAmmo()
{
	PrimaryActorTick.bCanEverTick = false;

    AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
    AmmoEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AmmoEffectComp"));

    SetRootComponent(AmmoMesh);
    AmmoEffectComp->SetupAttachment(AmmoMesh);

	HitPointComp = CreateDefaultSubobject<UAPHitPointComponent>(TEXT("HitPointComp"));

	AmmoMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("AmmoMoveComp"));
	AmmoMoveComp->MaxSpeed = AmmoSpeed;
	AmmoMoveComp->InitialSpeed = AmmoSpeed;
}

// Called when the game starts or when spawned
void ATurretAmmo::BeginPlay()
{
	Super::BeginPlay();
	
	BindHit();
}

// Called every frame
void ATurretAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurretAmmo::BindHit()
{
	AmmoMesh->OnComponentHit.AddDynamic(this, &ATurretAmmo::OnHitting);
	AmmoMesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ATurretAmmo::DestroyAmmo, DeadTime, false);
}

void ATurretAmmo::DestroyAmmo()
{
	auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, GetActorLocation(), GetActorRotation());
	Destroy();
}

void ATurretAmmo::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 

	auto OwnerCharacter = Cast<AArcanePunkCharacter>(MyOwner->GetOwner()); if(!OwnerCharacter) return;
	if (OtherActor && OtherActor != this && OtherActor != MyOwner && OtherActor != OwnerCharacter)
	{
		HitPointComp->DistinctHitPoint(Hit.Location, OtherActor);
		float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, OwnerCharacter->GetCurrentATK(), Hit, true);
		OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
	}
    
	DestroyAmmo();
}


