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
#include "Components/Character/APHitPointComponent.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Components/SkillActor/APSkillType.h"

// Sets default values
ASwordImpact::ASwordImpact()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ImpactComp = CreateDefaultSubobject<UBoxComponent>(TEXT("ImpactComp"));
	BaseEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BaseEffect"));
	HitPointComp = CreateDefaultSubobject<UAPHitPointComponent>(TEXT("HitPointComp"));
	SkillTypeComp = CreateDefaultSubobject<UAPSkillType>(TEXT("SkillTypeComp"));

	SetRootComponent(ImpactComp);
	BaseEffect->SetupAttachment(ImpactComp);

	ImpactMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ImpactMovementComponent"));
	ImpactMovementComponent->MaxSpeed = ImpactSpeed;
	ImpactMovementComponent->InitialSpeed = ImpactSpeed;
}

// Called when the game starts or when spawned
void ASwordImpact::BeginPlay()
{
	Super::BeginPlay();
	BintHit();
}

// Called every frame
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
		ImpactComp->OnComponentHit.AddDynamic(this, &ASwordImpact::OnHitting);
		ImpactComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}
	else
	{
		ImpactComp->OnComponentBeginOverlap.AddDynamic(this, &ASwordImpact::OnPenetrating);
		ImpactComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	}
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ASwordImpact::DestroyImpact, DestroyTime, false);
}

void ASwordImpact::DestroyImpact()
{
	Destroy();
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
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
		auto Character = Cast<AArcanePunkCharacter>(MyOwner);
		if (OtherActor && OtherActor != this && OtherActor != MyOwner && Character)
		{
			if(bStun) HitPointComp->SetCrowdControl(OtherActor, ECharacterState::Stun, StateTime);
			HitPointComp->DistinctHitPoint(HitResult.Location, OtherActor);
			UGameplayStatics::ApplyDamage(OtherActor, Character->GetFinalATK() * DamageCoefficient, MyOwnerInstigator, this, DamageTypeClass);
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
	auto Character = Cast<AArcanePunkCharacter>(OtherActor);
	if(Character) Character->GetCrowdControlComponent()->SlowState(SlowCoefficient, StateTime);
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

void ASwordImpact::SetSkillType(uint8 SkillType)
{
	SkillTypeComp->SetSkillType(SkillType, bStun, ImpactComp, ImpactMovementComponent);
}
