

#include "Skill/Install/APTurret.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameElements/DamageText/DamageText.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Skill/SkillActor.h"

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

    UE_LOG(LogTemp, Display, TEXT("Your BeginPlay"));
    OriginAmmoNum = SpawnAmmoNum;
    Enemy = FindEnemy();

    SetActorTickEnabled(true);
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AAPTurret::Fire, FireRate, true);
}

void AAPTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(Enemy.IsValid()) {AutoAttack(DeltaTime);}
    else {Enemy = FindEnemy();}
}

void AAPTurret::DestroySKill()
{
    Super::DestroySKill();
    if(DestroyEffect) auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DestroyEffect, GetActorLocation(), GetActorRotation());
	if(OwnerSkillActor.IsValid())
    {
        OwnerSkillActor->SetSkilling(false);
        OwnerSkillActor->StartCoolDown();  
    }
      
    // if(bDeadStun && OwnerCharacter.IsValid()) OwnerCharacter->GetAttackComponent()->MultiAttack_OnlyCC(GetActorLocation(), GetActorLocation() + GetActorUpVector() * 25.0f, DeadStunRadius, ECharacterState::Stun, StateTime);
}

void AAPTurret::InitInstall(ASkillActor *SkillActor)
{
    Super::InitInstall(SkillActor);
}

float AAPTurret::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
    if(Cast<AArcanePunkPlayerController>(EventInstigator)) return DamageApplied;

	DamageApplied = FMath::Min(HP, DamageApplied);
	HP = HP - DamageApplied;

    SpawnDamageText(EventInstigator, DamageAmount);
    if(HP <= KINDA_SMALL_NUMBER) DestroySKill();

    return DamageApplied;
}

void AAPTurret::Fire()
{
    if(!Enemy.IsValid()) return;

	auto Ammo = GetWorld()->SpawnActor<ATurretAmmo>(TurretAmmoClass, AmmoSpawnPoint->GetComponentLocation(), AmmoSpawnPoint->GetComponentRotation());
	Ammo->SetOwner(this);
    
    // SpawnAmmoNum--;
    // if(SpawnAmmoNum == 0) {SpawnAmmoNum = OriginAmmoNum; GetWorldTimerManager().ClearTimer(SpawnLoopTimerHandle);}
    // else {GetWorldTimerManager().SetTimer(SpawnLoopTimerHandle, this, &AArcaneTurret::Fire, SpawnLoopRate, false);}
}

void AAPTurret::AutoAttack(float DeltaTime)
{
    FVector ToTarget = Enemy->GetActorLocation() - RotateMesh->GetComponentLocation();
	FRotator TargetRotation = FRotator(0, ToTarget.Rotation().Yaw, 0);

	RotateMesh->SetWorldRotation(FMath::RInterpTo(RotateMesh->GetComponentRotation(), TargetRotation, DeltaTime, RotateSpeed));
}

AActor* AAPTurret::FindEnemy()
{
    TArray<AActor*> Actors; float Dist = AttackDistance;
	UGameplayStatics::GetAllActorsWithTag(this, TEXT("Enemy"), Actors);
	AActor* Target = UGameplayStatics::FindNearestActor(GetActorLocation(), Actors, Dist);
	if(Target)
	{
		if(Dist <= AttackDistance)
		{
			return Target;
		}
	}
    return nullptr;
}

void AAPTurret::SpawnDamageText(AController *EventInstigator, float Damage)
{
    ADamageText* DamageText = GetWorld()->SpawnActor<ADamageText>(DamageTextClass, GetActorLocation(), FRotator(0.0f, 90.0f, 0.0f)); if(!DamageText) return;
    DamageText->SetOwner(this);

    OwnerCharacter = Cast<AArcanePunkCharacter>(EventInstigator->GetPawn()); 
    auto InstigatorEnemy =  Cast<AEnemy_CharacterBase>(Enemy);

    if(OwnerCharacter.IsValid()) DamageText->SetDamageText(Damage, OwnerCharacter->IsCriticalAttack());
    else if(InstigatorEnemy) DamageText->SetDamageText(Damage, InstigatorEnemy->IsCriticalAttack());
}