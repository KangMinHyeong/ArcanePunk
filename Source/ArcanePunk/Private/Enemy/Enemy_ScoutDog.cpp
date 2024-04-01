
#include "Enemy/Enemy_ScoutDog.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

AEnemy_ScoutDog::AEnemy_ScoutDog()
{
    RushTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RushTrigger"));

    RushTrigger->SetupAttachment(GetMesh());
}

void AEnemy_ScoutDog::BeginPlay()
{
    Super::BeginPlay();

    RushTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RushTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_ScoutDog::OnOverlapping);
}

void AEnemy_ScoutDog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    CurrentLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, InterpSpeed);
    SetActorLocation(CurrentLocation);
    if(abs(CurrentLocation.X - TargetLocation.X )< 1.0f && abs(CurrentLocation.Y - TargetLocation.Y) < 1.0f) 
    {
        AttackCondition(false);
    }
}

void AEnemy_ScoutDog::LeapFoward()
{
    CurrentLocation = GetActorLocation(); 
    TargetLocation = CurrentLocation + LeapSpeed*GetActorForwardVector();
    AttackCondition(true);
}

void AEnemy_ScoutDog::SpawnJumpEffect()
{
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), JumpEffect, GetMesh()->GetComponentLocation() - FVector(0.0f,-30.0f,0.0f), GetMesh()->GetComponentRotation(), GetActorScale3D());
}

void AEnemy_ScoutDog::OnOverlapping(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    float Damage = Monster_ATK * CriticalCalculate(NormalAttack_CriticalMultiple);
	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag(TEXT("Player")))
	{
        FVector HitVector;
        FPointDamageEvent myDamageEvent(Damage, SweepResult, HitVector, nullptr);
		AController* MyController = Cast<AController>(GetController());
		DistinctHitPoint(OtherActor->GetActorLocation(), OtherActor);
        OtherActor->TakeDamage( Damage, myDamageEvent, MyController, this);
		if(bKnockBackAttack) OnPlayerKnockBack(OtherActor, KnockBackDist, KnockBackTime);
	}
}

void AEnemy_ScoutDog::AttackCondition(bool NewBool)
{
    if(NewBool)
    {
        GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
        GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
        RushTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
    else
    {
        GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
        GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
        RushTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
    }
    SetActorTickEnabled(NewBool);
}