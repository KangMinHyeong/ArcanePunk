// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SwordThrowBase.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Particles/ParticleSystem.h"
#include "Components/BoxComponent.h"
#include "Components/Common/APCrowdControlComponent.h"

// Sets default values
ASwordThrowBase::ASwordThrowBase()
{	
	Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailEffect"));
	HitTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("HitTrigger"));
	
	SetRootComponent(HitTrigger);
	Sword->SetupAttachment(HitTrigger);
	TrailEffect->SetupAttachment(HitTrigger);
	
	SwordMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("SwordMovementComponent"));
	SwordMovementComponent->MaxSpeed = SwordSpeed;
	SwordMovementComponent->InitialSpeed = SwordSpeed;

	SkillCategory = ESkillCategory::Projecitle;
}

// Called when the game starts or when spawned
void ASwordThrowBase::BeginPlay()
{
	Super::BeginPlay();

	HitTrigger->OnComponentHit.AddDynamic(this, &ASwordThrowBase::OnHitting);
	//HitTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

// Called every frame
void ASwordThrowBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwordThrowBase::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	OwnerCharacter = Cast<AArcanePunkCharacter>(MyOwner);
	
	if (OtherActor && OtherActor != this && OtherActor != MyOwner && OwnerCharacter.IsValid())
	{
		if(bStun) HitPointComp->SetCrowdControl(OtherActor, ECharacterState::Stun, StateTime);
		HitPointComp->DistinctHitPoint(Hit.Location, OtherActor);
		
		float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, Hit, true);
		OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
		// UGameplayStatics::ApplyDamage(OtherActor, , MyOwnerInstigator, this, DamageTypeClass);
		if(HitEffect && OtherActor->ActorHasTag(TEXT("Enemy")))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, OtherActor->GetActorLocation(), OtherActor->GetActorRotation(), FVector(0.2f,0.2f,0.2f));
		}
	}
}

void ASwordThrowBase::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData)
{
    Super::SetSkill(SkillAbilityNestingData);
	HitTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
}
