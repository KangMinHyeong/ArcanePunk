
#include "Skill/Sub/TurretAmmo.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Skill/ArcaneTurret.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/Character/APHitPointComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Character/ArcanePunkCharacter.h"

ATurretAmmo::ATurretAmmo()
{
	PrimaryActorTick.bCanEverTick = false;

    AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
    AmmoEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AmmoEffect"));

    SetRootComponent(AmmoMesh);
    AmmoEffect->SetupAttachment(AmmoMesh);

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
	if(HitEffet) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffet, GetActorLocation(), GetActorRotation(), FVector(0.5f, 0.5f, 0.5f));
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
	auto MyOwnerInstigator = MyOwner->GetOwner()->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	auto ArcaneTurret = Cast<AArcaneTurret>(MyOwner);
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(MyOwner->GetOwner()); if(!OwnerCharacter) return;
	if (OtherActor && OtherActor != this && OtherActor != MyOwner && ArcaneTurret)
	{
		HitPointComp->DistinctHitPoint(Hit.Location, OtherActor);
		float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, ArcaneTurret->GetDamage(), Hit, true);
		OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
		// UGameplayStatics::ApplyDamage(OtherActor, ArcaneTurret->GetDamage() * OwnerCharacter->CriticalCalculate(), MyOwnerInstigator, MyOwner->GetOwner(), DamageTypeClass);
        if(HitEffet) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffet, GetActorLocation(), GetActorRotation(), FVector(0.5f, 0.5f, 0.5f));
	}
    Destroy();
}


