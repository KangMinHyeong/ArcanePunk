
#include "GameElements/Trap/APTrapBase_CrossBow.h"

#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GameElements/Projectile/APProjectileBase.h"

AAPTrapBase_CrossBow::AAPTrapBase_CrossBow()
{
 	PrimaryActorTick.bCanEverTick = true;

    RangeEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RangeEffect"));
    ArrowSpawnComp = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowSpawnComp"));

	RangeEffect->SetupAttachment(RotateMesh);
    ArrowSpawnComp->SetupAttachment(RotateMesh);
}

void AAPTrapBase_CrossBow::BeginPlay()
{
	Super::BeginPlay();
    
    ShootRange.X = TrapTrigger->GetScaledSphereRadius();
    RangeEffect->DeactivateImmediate(); 
    TrapTrigger->OnComponentEndOverlap.AddDynamic(this, &AAPTrapBase_CrossBow::OnOverlapEnd);
}

void AAPTrapBase_CrossBow::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if(TrapTrigger->IsOverlappingActor(Player.Get()) && bRotating) AutoRotating();
}

void AAPTrapBase_CrossBow::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if(Cast<AArcanePunkCharacter>(OtherActor)) Player = Cast<AArcanePunkCharacter>(OtherActor);
    if(!Player.IsValid()) return;

    bRotating = true;

    GetWorldTimerManager().SetTimer(TimerHandle, this, &AAPTrapBase_CrossBow::ReadyToShoot, TrapOperationTime, false);
}

void AAPTrapBase_CrossBow::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    if(!Cast<AArcanePunkCharacter>(OtherActor)) return;

    bRotating = false;

    GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AAPTrapBase_CrossBow::ReadyToShoot()
{
    if(!TrapTrigger->IsOverlappingActor(Player.Get())) return;

    GetWorldTimerManager().ClearTimer(TimerHandle);

    bRotating = false;
    
    RangeEffect->SetVariableVec2(TEXT("Size2D"), FVector2D(ShootRange.X / 1000.0f, ShootRange.Y / 200.0f));
    RangeEffect->Activate(); 

    GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &AAPTrapBase_CrossBow::ShootArrow, ShootDelayTime, false);
}

void AAPTrapBase_CrossBow::ShootArrow()
{
    auto Ammo = GetWorld()->SpawnActor<AAPProjectileBase>(ProjectileClass, ArrowSpawnComp->GetComponentLocation(), ArrowSpawnComp->GetComponentRotation());
    if(!Ammo) return;
    Ammo->SetDestroy(ShootRange.X / ArrowSpeed);
    Ammo->SetSpeed(ArrowSpeed);
    Ammo->SetDamage(TrapDamage);
    Ammo->SetRadius(ShootRange.Y * 0.5f);

    RangeEffect->DeactivateImmediate(); 

    if(!TrapTrigger->IsOverlappingActor(Player.Get())) return;

    bRotating = true;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AAPTrapBase_CrossBow::ReadyToShoot, TrapOperationTime, false);
}