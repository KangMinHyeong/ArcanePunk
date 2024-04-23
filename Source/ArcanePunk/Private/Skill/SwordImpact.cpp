// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SwordImpact.h"

#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemy/Enemy_CharacterBase.h"

ASwordImpact::ASwordImpact()
{
	ImpactComp = CreateDefaultSubobject<UBoxComponent>(TEXT("ImpactComp"));
	BaseEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BaseEffect"));

	SetRootComponent(ImpactComp);
	BaseEffect->SetupAttachment(ImpactComp);

	ImpactMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ImpactMovementComponent"));
	ImpactMovementComponent->MaxSpeed = ImpactSpeed;
	ImpactMovementComponent->InitialSpeed = ImpactSpeed;

	SkillCategory = ESkillCategory::Projecitle;
}

void ASwordImpact::BeginPlay()
{
	Super::BeginPlay();
	SlowPercent = 50;
}

void ASwordImpact::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwordImpact::BintHit()
{
	//SetObjectType();
	//ECollisionChannel 
	if(!IsPenetrate)
	{
		ImpactComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		ImpactComp->OnComponentHit.AddDynamic(this, &ASwordImpact::OnHitting);
	}
	else
	{
		ImpactComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		ImpactComp->OnComponentBeginOverlap.AddDynamic(this, &ASwordImpact::OnPenetrating);
	}

}

void ASwordImpact::DamageAction(AActor *OtherActor, const FHitResult &HitResult)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	if(IsPlayerSkill)
	{
		OwnerCharacter = Cast<AArcanePunkCharacter>(MyOwner);
		if (OtherActor && OtherActor != this && OtherActor != MyOwner && OwnerCharacter.IsValid())
		{
			if(bStun) HitPointComp->SetCrowdControl(OtherActor, ECharacterState::Stun, StateTime);
			HitPointComp->DistinctHitPoint(HitResult.Location, OtherActor);
			
			float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, HitResult, true);
		    OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
			// UGameplayStatics::ApplyDamage(OtherActor, OwnerCharacter->GetCurrentATK()* OwnerCharacter->CriticalCalculate() * DamageCoefficient, MyOwnerInstigator, this, DamageTypeClass);
			if(HitEffect && OtherActor->ActorHasTag(TEXT("Enemy")))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, OtherActor->GetActorLocation(), OtherActor->GetActorRotation(), FVector(0.2f,0.2f,0.2f));
			}
		}
	}
	else
	{
		auto Enemy = Cast<AEnemy_CharacterBase>(MyOwner);
		if (OtherActor && OtherActor != this && OtherActor != MyOwner && Enemy)
		{
			if(HitEffect && OtherActor->ActorHasTag(TEXT("Player")))
			{
				Enemy->DistinctHitPoint(HitResult.Location, OtherActor);
				UGameplayStatics::ApplyDamage(OtherActor, Enemy->GetMonsterATK() * DamageCoefficient, MyOwnerInstigator, this, DamageTypeClass);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, OtherActor->GetActorLocation(), OtherActor->GetActorRotation(), FVector(0.2f,0.2f,0.2f));
				SlowPlayer(OtherActor);
			}
		}
	}

}

void ASwordImpact::SlowPlayer(AActor *OtherActor)
{
	OwnerCharacter = Cast<AArcanePunkCharacter>(OtherActor);
	if(OwnerCharacter.IsValid()) OwnerCharacter->GetCrowdControlComponent()->SlowState(SlowPercent, StateTime);
}

void ASwordImpact::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	DamageAction(OtherActor, Hit);
	Destroy();
}

void ASwordImpact::OnPenetrating(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	DamageAction(OtherActor, SweepResult);
}

void ASwordImpact::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData)
{
    Super::SetSkill(SkillAbilityNestingData);

	BaseEffect->SetNiagaraVariableLinearColor(TEXT("Color"),  EffectColor);
	BintHit();
	ImpactComp->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
}
