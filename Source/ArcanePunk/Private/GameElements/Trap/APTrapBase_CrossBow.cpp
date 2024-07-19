
#include "GameElements/Trap/APTrapBase_CrossBow.h"

#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

AAPTrapBase_CrossBow::AAPTrapBase_CrossBow()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void AAPTrapBase_CrossBow::BeginPlay()
{
	Super::BeginPlay();
    
    TrapTrigger->OnComponentEndOverlap.AddDynamic(this, &AAPTrapBase_CrossBow::OnOverlapEnd);
}

void AAPTrapBase_CrossBow::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if(TrapTrigger->IsOverlappingActor(Player.Get()) && bRotating) AutoRotating();
}

void AAPTrapBase_CrossBow::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    Player = Cast<AArcanePunkCharacter>(OtherActor); if(!Player.IsValid()) return;

    GetWorldTimerManager().SetTimer(TimerHandle, this, &AAPTrapBase_CrossBow::ReadyToShoot, TrapOperationTime, false);
}

void AAPTrapBase_CrossBow::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    Player = Cast<AArcanePunkCharacter>(OtherActor); if(!Player.IsValid()) return;

    GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AAPTrapBase_CrossBow::ReadyToShoot()
{
    if(!TrapTrigger->IsOverlappingActor(Player.Get())) return;

    GetWorldTimerManager().ClearTimer(TimerHandle);

    bRotating = false;
    // RangeDecal 
    // ShootDelay
}

void AAPTrapBase_CrossBow::ShootArrow()
{
    // Shoot

    if(!TrapTrigger->IsOverlappingActor(Player.Get())) return;

    bRotating = true;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AAPTrapBase_CrossBow::ReadyToShoot, TrapOperationTime, false);
}