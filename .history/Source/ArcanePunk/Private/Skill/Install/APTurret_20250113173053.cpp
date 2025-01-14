

#include "Skill/Install/APTurret.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/Sub/TurretAmmo.h"

AAPTurret::AAPTurret()
{
    RotateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateMesh"));
    AmmoSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));

    RotateMesh->SetupAttachment(InstallRoot);
    AmmoSpawnPoint->SetupAttachment(RotateMesh);
}

void AAPTurret::BeginPlay()
{
    Super::BeginPlay();

    OriginAmmoNum = SpawnAmmoNum;
    Enemy = FindEnemy();

    SetActorTickEnabled(true);
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AArcaneTurret::Fire, FireRate, true);
}

void AAPTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAPTurret::DestroySKill()
{
    Super::DestroySKill();
    DestroyEffect
}

void AAPTurret::InitInstall(ASkillActor *SkillActor)
{
}

float AAPTurret::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
    if(Cast<AArcanePunkPlayerController>(EventInstigator)) return DamageApplied;

	DamageApplied = FMath::Min(HP, DamageApplied);
	HP = HP - DamageApplied;

    SpawnDamageText(EventInstigator, DamageAmount);
    if(HP <= KINDA_SMALL_NUMBER)
	{
	 	DestroySKill();
	}

    return DamageApplied;
}

void AAPTurret::Fire()
{
    if(!Enemy.IsValid()) return;

	auto Ammo = GetWorld()->SpawnActor<ATurretAmmo>(TurretAmmoClass, AmmoSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
	Ammo->SetOwner(this);
    SpawnAmmoNum--;

    if(bEnhance)
    {
        EnhanceCount++;
        if(EnhanceCount == 3) {EnhanceCount = 0; GetWorldTimerManager().SetTimer(EnhanceTimerHandle, this, &AArcaneTurret::EnhanceFire, EnhanceTerm, false);}
    }
    
    if(SpawnAmmoNum == 0) {SpawnAmmoNum = OriginAmmoNum; GetWorldTimerManager().ClearTimer(SpawnLoopTimerHandle);}
    else {GetWorldTimerManager().SetTimer(SpawnLoopTimerHandle, this, &AArcaneTurret::Fire, SpawnLoopRate, false);}
}