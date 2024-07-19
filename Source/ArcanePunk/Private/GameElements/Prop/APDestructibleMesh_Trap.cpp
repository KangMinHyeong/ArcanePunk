
#include "GameElements/Prop/APDestructibleMesh_Trap.h"

#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

void AAPDestructibleMesh_Trap::BeginPlay()
{
    Super::BeginPlay();
	
	APDestructionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAPDestructibleMesh_Trap::OnOverlap);
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AAPDestructibleMesh_Trap::AutoDestroy, DestroyTime, false);
}

float AAPDestructibleMesh_Trap::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
    return 0.0f;
}

void AAPDestructibleMesh_Trap::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	UGameplayStatics::ApplyDamage(Character, TrapDamage, nullptr, this, UDamageType::StaticClass());

	auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TrapOverlapEffect, GetActorLocation(), GetActorRotation());
	Destroy();
}

void AAPDestructibleMesh_Trap::AutoDestroy()
{
    Explode(GetActorLocation());
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AAPDestructibleMeshBase::StartFadeOut, FadeOutTime, false);
}
