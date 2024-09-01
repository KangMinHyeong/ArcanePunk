
#include "Enemy/Enemy_Drone.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"

AEnemy_Drone::AEnemy_Drone()
{
    DestroyPoint = CreateDefaultSubobject<USceneComponent>(TEXT("DestroyPoint"));
    DestroyPoint->SetupAttachment(GetMesh(), TEXT("DestroyPoint"));

    Basic_MOVE = EMovementMode::MOVE_Flying;
    GetCharacterMovement()->SetMovementMode(Basic_MOVE);
}

void AEnemy_Drone::BeginPlay()
{
    Super::BeginPlay();

}

void AEnemy_Drone::NormalAttack()
{
    Super::NormalAttack();
    auto Loc = DestroyPoint->GetComponentLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
    auto Rot = GetActorRotation(); Rot.Pitch -= 6.5f;
    auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), LaserEffect, Loc, Rot);
}

void AEnemy_Drone::OnDeath_MontageEnded()
{
    Super::OnDeath_MontageEnded();

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DestroyEffect, DestroyPoint->GetComponentLocation(), GetActorRotation(), FVector(3.0f, 3.0f, 3.0f));
	EnemyDestroyed();
    // USkeletalMeshComponent 
}
