
#include "Enemy/Enemy_ScoutDog.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemy_ScoutDog::AEnemy_ScoutDog()
{
    RushTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RushTrigger"));

    RushTrigger->SetupAttachment(GetMesh());
}

void AEnemy_ScoutDog::BeginPlay()
{
    Super::BeginPlay();

    RushTrigger->SetCapsuleRadius(Monster_AttackRadius, false);
    RushTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RushTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_ScoutDog::OnOverlapping);
    FTimerHandle DamageTimerHandle;
    // GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &AEnemy_ScoutDog::Test, 0.3f, true);
}

void AEnemy_ScoutDog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if(IsDead()) return;
    CurrentLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, InterpSpeed);
    SetActorLocation(CurrentLocation, true);
    if(abs(CurrentLocation.X - TargetLocation.X )< 1.0f && abs(CurrentLocation.Y - TargetLocation.Y) < 1.0f) 
    {
        AttackCondition(false);
    }
}

float AEnemy_ScoutDog::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
    float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if(DamageApplied <= KINDA_SMALL_NUMBER)
    AttackCondition(false);

    return DamageApplied;
}

void AEnemy_ScoutDog::LeapFoward()
{
    CurrentLocation = GetActorLocation(); 
    TargetLocation = CurrentLocation + (Monster_AttackRange_Plus + Monster_AttackRadius) * GetActorForwardVector();
    AttackCondition(true);
}

void AEnemy_ScoutDog::SpawnJumpEffect()
{
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), JumpEffect, GetMesh()->GetComponentLocation() - FVector(0.0f,-30.0f,0.0f), GetMesh()->GetComponentRotation(), GetActorScale3D());
}

void AEnemy_ScoutDog::OnOverlapping(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if(IsDead()) return;
    float Damage = TotalStatus.StatusData.ATK * CriticalCalculate();
	if (OtherActor && OtherActor != this)
	{
        FHitResult Hit;
	    Hit.Location = GetActorLocation(); Hit.ImpactPoint = Hit.Location;
        FPointDamageEvent myDamageEvent(Damage, Hit, Hit.ImpactPoint, nullptr);
		DistinctHitPoint(OtherActor->GetActorLocation(), OtherActor);
        OtherActor->TakeDamage( Damage, myDamageEvent, GetController(), this);
		if(bKnockBackAttack) OnPlayerKnockBack(OtherActor, KnockBackDist, KnockBackTime);
	}
}

void AEnemy_ScoutDog::AttackCondition(bool NewBool)
{
    if(NewBool)
    {
        RushTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
    else 
    {
        RushTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
    }
    SetCapsuleOverlap(NewBool);
    SetActorTickEnabled(NewBool);
}