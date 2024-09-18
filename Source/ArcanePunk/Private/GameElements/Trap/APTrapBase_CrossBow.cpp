
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
#include "AnimInstance/APTrapAnimInstance.h"

AAPTrapBase_CrossBow::AAPTrapBase_CrossBow()
{
 	PrimaryActorTick.bCanEverTick = true;

    bRotating = false;
   
    ArrowComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArrowComp"));
    ArrowComp->SetupAttachment(TopMesh);

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
        ShootRange.Y = StaticCollision->GetScaledBoxExtent().X * 1.0f;
        StaticCollision->OnComponentEndOverlap.AddDynamic(this, &AAPTrapBase_CrossBow::OnOverlapEnd);
    }
    else
    {
        ShootRange.X = TrapCollision->GetScaledSphereRadius();
        TrapCollision->OnComponentEndOverlap.AddDynamic(this, &AAPTrapBase_CrossBow::OnOverlapEnd);
    }

    auto Anim = Cast<UAPTrapAnimInstance>(TopMesh->GetAnimInstance());
	if(Anim) Anim->OnMontageEnded.AddDynamic(this, &AAPTrapBase::TrapMontageEnded);

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

void AAPTrapBase_CrossBow::Deactivate()
{
    Super::Deactivate();
    StaticCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AAPTrapBase_CrossBow::OnOverlap);
    StaticCollision->OnComponentEndOverlap.RemoveDynamic(this, &AAPTrapBase_CrossBow::OnOverlapEnd);
    TrapCollision->OnComponentEndOverlap.RemoveDynamic(this, &AAPTrapBase_CrossBow::OnOverlapEnd);
}

void AAPTrapBase_CrossBow::ReadyToShoot()
{
    GetWorldTimerManager().ClearTimer(TimerHandle);
    if(!bActivate) return;
    float DelayTime = ShootDelayTime;

    auto Anim = Cast<UAPTrapAnimInstance>(TopMesh->GetAnimInstance()); 
	if(Anim) DelayTime += Anim->PlayReload_Montage();

    Anim = Cast<UAPTrapAnimInstance>(ArrowComp->GetAnimInstance());
	if(Anim) Anim->PlayReload_Montage();
    
    bOperationEnd = true;

    if(bStatic)
    {
        if(!StaticCollision->IsOverlappingActor(Player.Get())) return;
    }
    else
    {
        if(!TrapCollision->IsOverlappingActor(Player.Get())) return;
        bRotating = false;
    }
    
    FRotator Rot = TopMesh->GetComponentRotation(); Rot.Yaw -= RotatePlus;
    auto TrapRange = GetWorld()->SpawnActor<AAPEnemyAttackRange>(RangeClass, GetActorLocation(), Rot); if(!TrapRange) return;
	TrapRange->SetDecalSize(ShootRange.Y, ShootRange.X + ShootRange.Y * 0.5f, DelayTime);
        
    GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &AAPTrapBase_CrossBow::Shoot, DelayTime, false);
}

void AAPTrapBase_CrossBow::Shoot()
{
    auto Anim = Cast<UAPTrapAnimInstance>(TopMesh->GetAnimInstance());
	if(Anim) Anim->PlayTrapOperation_Montage();

    Anim = Cast<UAPTrapAnimInstance>(ArrowComp->GetAnimInstance());
	if(Anim) Anim->PlayTrapOperation_Montage();
}

void AAPTrapBase_CrossBow::TrapMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
    // Super::TrapMontageEnded(Montage, bInterrupted);
    auto Anim_Top = Cast<UAPTrapAnimInstance>(TopMesh->GetAnimInstance()); if(!Anim_Top) return;
	if(Montage == Anim_Top->TrapOperation_Montage) OnTrapOperation_MontageEnded();
}

void AAPTrapBase_CrossBow::OnDamageTrigger()
{
    if(!bActivate) return;
    auto Rot = TopMesh->GetComponentRotation(); Rot.Yaw -= RotatePlus;
    auto Ammo = GetWorld()->SpawnActor<AAPProjectileBase>(ProjectileClass, ArrowComp->GetComponentLocation(), Rot);
    if(!Ammo) return;
    Ammo->SetOwner(this);
    Ammo->SetDestroy((ShootRange.X - ShootRange.Y) / ArrowSpeed);
    Ammo->SetSpeed(ArrowSpeed);
    Ammo->SetDamage(TrapDamage);
    Ammo->SetRadius(ShootRange.Y);    

    bOperationEnd = false;
}

void AAPTrapBase_CrossBow::OnTrapOperation_MontageEnded()
{
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