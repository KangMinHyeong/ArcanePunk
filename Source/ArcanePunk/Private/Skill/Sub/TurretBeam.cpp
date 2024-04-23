
#include "Skill/Sub/TurretBeam.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Skill/ArcaneTurret.h"
#include "Components/Character/APHitPointComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APAttackComponent.h"

ATurretBeam::ATurretBeam()
{
	PrimaryActorTick.bCanEverTick = false;

	BeamTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("BeamTrigger"));
    BeamEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BeamEffect"));

    SetRootComponent(BeamTrigger);
    BeamEffect->SetupAttachment(BeamTrigger);

	HitPointComp = CreateDefaultSubobject<UAPHitPointComponent>(TEXT("HitPointComp"));

	BeamMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BeamMovementComponent"));
	BeamMovementComponent->MaxSpeed = BeamSpeed;
	BeamMovementComponent->InitialSpeed = BeamSpeed;
}

void ATurretBeam::BeginPlay()
{
	Super::BeginPlay();
	BeamTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATurretBeam::OnOverlap);
	BeamTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ATurretBeam::DestroyBeam, DeadTime, false);
}

void ATurretBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurretBeam::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
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
		HitPointComp->DistinctHitPoint(OtherActor->GetActorLocation(), OtherActor);
		float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, ArcaneTurret->GetDamage() * EnhanceCoefficient, SweepResult, true);
		OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
		// UGameplayStatics::ApplyDamage(OtherActor, ArcaneTurret->GetDamage()* OwnerCharacter->CriticalCalculate() * EnhanceCoefficient , MyOwnerInstigator, MyOwner->GetOwner(), DamageTypeClass);
	}
}

void ATurretBeam::DestroyBeam()
{
	Destroy();
}
