// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/ArcaneExecution.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

AArcaneExecution::AArcaneExecution()
{
    ExecutionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExecutionSphere"));
    
    SetRootComponent(ExecutionSphere);
}

void AArcaneExecution::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AArcaneExecution::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(!OwnerCharacter.IsValid()) return;

    FVector Current = OwnerCharacter->GetActorLocation();

    if(AttackType == EAttackType::First)
    {
        Current = FMath::VInterpConstantTo(Current, TargetEnemyLocation, DeltaTime, FirstAttackSpeed);
        OwnerCharacter->SetActorLocation(Current);

        if(abs(Current.X - TargetEnemyLocation.X) < 0.1f && abs(Current.Y - TargetEnemyLocation.Y) < 0.1f)
        {
            AttackType = EAttackType::None;
            Attack_First();
        }
    }
    else if(AttackType == EAttackType::Second)
    {
        Current = FMath::VInterpConstantTo(Current, TargetEnemyLocation, DeltaTime, SecondAttackSpeed);
        OwnerCharacter->SetActorLocation(Current);

        if(abs(Current.X - TargetEnemyLocation.X) < 0.1f && abs(Current.Y - TargetEnemyLocation.Y) < 0.1f)
        {
            AttackType = EAttackType::None;
            SetPlayerCollisionEnable(true); DeActivate_Ult();
            DestroySKill();
        }
    }
}

void AArcaneExecution::Attack_First()
{
    if(!OwnerCharacter.IsValid()) return;

    DeActivate_Ult(); SetPlayerCollisionEnable(true);
    
    if(TargetEnemy)
    {
        FHitResult Hit;
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FirstEffect, TargetEnemy->GetActorLocation(), TargetEnemy->GetActorRotation());
        float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(TargetEnemy, OwnerCharacter->GetCurrentATK() * DamageCoefficient, Hit, true);
		OwnerCharacter->GetAttackComponent()->DrainCheck(TargetEnemy, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
    }
}

void AArcaneExecution::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetOwner()->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner && OwnerCharacter.IsValid())
	{
		float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, SweepResult, true);
		OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
	}
}

void AArcaneExecution::SetExecutionRadius(float Radius)
{
    // ExecutionSphere->SetSphereRadius(Radius);
    SetActorScale3D(GetActorScale3D() * (Radius / ExecutionSphere->GetScaledSphereRadius())); 
}

void AArcaneExecution::DestroySKill() 
{
    Super::DestroySKill();
}

void AArcaneExecution::SetExecutionType(uint8 NewValue)
{
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); 

    if(NewValue == 1)
    {
        AttackType = EAttackType::First;

        if(TargetEnemy)
        {
            FVector Unit = (TargetEnemy->GetMesh()->GetComponentLocation() - OwnerCharacter->GetMesh()->GetComponentLocation()); Unit = Unit / Unit.Size();
            TargetEnemyLocation = TargetEnemy->GetMesh()->GetComponentLocation() + Unit * PlusDist; TargetEnemyLocation.Z += OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
        } 
    }
    else if(NewValue == 2)
    {
        TargetEnemyLocation += OwnerCharacter->GetMesh()->GetComponentLocation(); TargetEnemyLocation.Z += OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
        AttackType = EAttackType::Second;
        ExecutionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
        auto NC = UNiagaraFunctionLibrary::SpawnSystemAttached(SecondEffect, OwnerCharacter->GetMesh(), TEXT("SecondEffect"), OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetMesh()->GetComponentRotation(), FVector(1,1,1), EAttachLocation::KeepWorldPosition, true, ENCPoolMethod::None, true);
        NC->SetVariableFloat(TEXT("Time"), (TargetEnemyLocation - OwnerCharacter->GetActorLocation() ).Size() / SecondAttackSpeed);
        SetPlayerCollisionEnable(false);
    }
}

void AArcaneExecution::SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);

    if(!OwnerCharacter.IsValid()) return;
    
    SetPlayerCollisionEnable(false);
    ExecutionSphere->OnComponentBeginOverlap.AddDynamic(this, &AArcaneExecution::OnOverlap);
}
