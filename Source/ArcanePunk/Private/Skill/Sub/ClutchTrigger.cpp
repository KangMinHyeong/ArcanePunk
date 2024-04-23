
#include "Skill/Sub/ClutchTrigger.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/Character/APHitPointComponent.h"
#include "Skill/SwordClutch.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Interfaces/SkillInterface.h"
#include "Enemy/Enemy_CharacterBase.h"

AClutchTrigger::AClutchTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	ClutchSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ClutchSphere"));
	ClutchEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ClutchEffect"));

	HitPointComp = CreateDefaultSubobject<UAPHitPointComponent>(TEXT("HitPointComp"));

	SetRootComponent(ClutchSphere);
	ClutchEffect->SetupAttachment(ClutchSphere);
}

void AClutchTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void AClutchTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClutchTrigger::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetOwner()->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	auto SkillActor = Cast<AAPSkillActorBase>(MyOwner);
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(MyOwner->GetOwner()); if(!OwnerCharacter) return;
	if (OtherActor && OtherActor != this && OtherActor != MyOwner && SkillActor)
	{
		HitPointComp->DistinctHitPoint(OtherActor->GetActorLocation(), OtherActor);
		float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, SkillActor->GetDamage(), SweepResult, true);
		CheckEnemyDeath(OtherActor);
		if(SkillActor->IsSlow()) HitPointComp->SetCrowdControl(OtherActor, ECharacterState::Slow, SkillActor->GetStateTime());
		DrainCheck(OtherActor, DamageApplied);
		
        // if(HitEffet) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffet, GetActorLocation(), GetActorRotation(), FVector(0.5f, 0.5f, 0.5f));
	}
}

void AClutchTrigger::SetSlowPercent(uint8 SlowPercent)
{
	HitPointComp->SetSlowPercent(SlowPercent);
}

void AClutchTrigger::CheckEnemyDeath(AActor* OtherActor)
{
	auto SwordClutch = Cast<ASwordClutch>(GetOwner()); if(!SwordClutch) return;
	if(!SwordClutch->IsDurationUpgrade()) return;
	auto Enemy = Cast<AEnemy_CharacterBase>(OtherActor); if(!Enemy) return;
	if(Enemy->GetMonsterHP() < 0.001f)
	{
		auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()->GetOwner()); if(!OwnerCharacter) return;
		OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_14)->SetAddDuration(SwordClutch->GetDurationUpgradeCoefficient());
	}
}

void AClutchTrigger::DrainCheck(AActor* OtherActor, float DamageApplied)
{
	auto SkillActor = Cast<AAPSkillActorBase>(GetOwner()); if(!SkillActor) return;
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()->GetOwner()); if(!OwnerCharacter) return;

	float Coeff = OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient();
	if(auto SwordClutch = Cast<ASwordClutch>(SkillActor))
	{
		if(SwordClutch->IsDrain()) Coeff = FMath::Max(Coeff, SwordClutch->GetDrainCoefficient());
	}
	
	OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, Coeff);
}

void AClutchTrigger::SetClutchSphere(float NewValue)
{
	SetActorScale3D(GetActorScale3D() * NewValue / ClutchSphere->GetScaledSphereRadius());
	ClutchSphere->OnComponentBeginOverlap.AddDynamic(this, &AClutchTrigger::OnOverlap);
	ClutchSphere->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AClutchTrigger::SetDestroy(float Time)
{
	FTimerHandle DestroyTimerHandle;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AClutchTrigger::OnDestroy, Time, false);
}

void AClutchTrigger::OnDestroy()
{
	Destroy();
}
