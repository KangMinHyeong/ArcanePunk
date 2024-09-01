
#include "GameElements/Trap/APTrapBase_Mine.h"

#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemy/SkillActor/APEnemyAttackRange.h"

AAPTrapBase_Mine::AAPTrapBase_Mine()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAPTrapBase_Mine::BeginPlay()
{
    Super::BeginPlay();

}

void AAPTrapBase_Mine::OnTrapOperation_MontageEnded()
{
}

void AAPTrapBase_Mine::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAPTrapBase_Mine::OnDamageTrigger()
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors, AArcanePunkCharacter::StaticClass());

    for(AActor* Actor : Actors)
    {
        UGameplayStatics::ApplyDamage(Actor, TrapDamage, nullptr, this, UDamageType::StaticClass());
    }

    auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TrapOverlapEffect, GetActorLocation(), GetActorRotation());
    NC->SetVariableFloat(TEXT("Size"), 1.5f);
    Destroy();
}

void AAPTrapBase_Mine::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    Super::OnOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    if(!Player.IsValid()) return;

    UE_LOG(LogTemp, Display, TEXT("OperationTime %f"), OperationTime);
    auto TrapRange = GetWorld()->SpawnActor<AAPEnemyAttackRange>(RangeClass, GetActorLocation(), GetActorRotation()); if(!TrapRange) return;
	TrapRange->SetDecalSize(TrapCollision->GetScaledSphereRadius(), TrapCollision->GetScaledSphereRadius(), OperationTime, false);
}