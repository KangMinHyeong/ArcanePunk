
#include "GameElements/Trap/APTrapBase_CrossBow.h"

#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GameElements/Projectile/APProjectileBase.h"
#include "Enemy/SkillActor/APEnemyAttackRange.h"

AAPTrapBase_CrossBow::AAPTrapBase_CrossBow()
{
 	PrimaryActorTick.bCanEverTick = true;

    ArrowSpawnComp = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowSpawnComp"));
    ArrowSpawnComp->SetupAttachment(TopMesh);

    StaticCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("StaticCollision"));
    StaticCollision->SetupAttachment(GetRootComponent());
}

void AAPTrapBase_CrossBow::BeginPlay()
{
	Super::BeginPlay();
    
    if(bStatic)
    {
        TrapCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AAPTrapBase_CrossBow::OnOverlap);
        StaticCollision->OnComponentBeginOverlap.AddDynamic(this, &AAPTrapBase_CrossBow::OnOverlap);
        ShootRange.X = StaticCollision->GetScaledBoxExtent().Y * 2.0f;
        StaticCollision->OnComponentEndOverlap.AddDynamic(this, &AAPTrapBase_CrossBow::OnOverlapEnd);
    }
    else
    {
        ShootRange.X = TrapCollision->GetScaledSphereRadius();
        TrapCollision->OnComponentEndOverlap.AddDynamic(this, &AAPTrapBase_CrossBow::OnOverlapEnd);
    }
}

void AAPTrapBase_CrossBow::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if(TrapCollision->IsOverlappingActor(Player.Get()) && bRotating) AutoRotating();
}

void AAPTrapBase_CrossBow::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if(!Cast<AArcanePunkCharacter>(OtherActor)) return;
    Player = Cast<AArcanePunkCharacter>(OtherActor);

    if(!bStatic) bRotating = true;

    GetWorldTimerManager().SetTimer(TimerHandle, this, &AAPTrapBase_CrossBow::ReadyToShoot, TrapOperationTime, false);
}

void AAPTrapBase_CrossBow::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    if(!Cast<AArcanePunkCharacter>(OtherActor)) return;

    if(!bStatic) bRotating = false;

    GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AAPTrapBase_CrossBow::ReadyToShoot()
{
    GetWorldTimerManager().ClearTimer(TimerHandle);
    if(bStatic)
    {
        if(!StaticCollision->IsOverlappingActor(Player.Get())) return;
    }
    else
    {
        if(!TrapCollision->IsOverlappingActor(Player.Get())) return;
        bRotating = true;
    }
    
    FRotator Rot = TopMesh->GetComponentRotation(); Rot.Yaw -= RotatePlus;
    auto TrapRange = GetWorld()->SpawnActor<AAPEnemyAttackRange>(RangeClass, GetActorLocation(), Rot); if(!TrapRange) return;
	TrapRange->SetDecalSize(ShootRange.Y, ShootRange.X, ShootDelayTime);
        
    GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &AAPTrapBase_CrossBow::ShootArrow, ShootDelayTime, false);
}

void AAPTrapBase_CrossBow::ShootArrow()
{
    auto Ammo = GetWorld()->SpawnActor<AAPProjectileBase>(ProjectileClass, ArrowSpawnComp->GetComponentLocation(), ArrowSpawnComp->GetComponentRotation());
    if(!Ammo) return;
    Ammo->SetDestroy(ShootRange.X / ArrowSpeed);
    Ammo->SetSpeed(ArrowSpeed);
    Ammo->SetDamage(TrapDamage);
    Ammo->SetRadius(ShootRange.Y);


    if(bStatic)
    {
        if(!StaticCollision->IsOverlappingActor(Player.Get())) return;
    }
    else
    {
        if(!TrapCollision->IsOverlappingActor(Player.Get())) return;
        bRotating = true;
    }
     
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AAPTrapBase_CrossBow::ReadyToShoot, TrapOperationTime, false);
}