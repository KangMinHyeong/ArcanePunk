
#include "Enemy/Enemy_Drone.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

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
}

void AEnemy_Drone::OnDeath_MontageEnded()
{
    Super::OnDeath_MontageEnded();

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DestroyEffect, DestroyPoint->GetComponentLocation(), GetActorRotation(), FVector(2.0f, 2.0f, 2.0f));
	EnemyDestroyed();
    // USkeletalMeshComponent 
}
