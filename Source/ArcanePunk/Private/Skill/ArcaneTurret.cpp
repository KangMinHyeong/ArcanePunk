
#include "Skill/ArcaneTurret.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DamageText/DamageText.h"
#include "Particles/ParticleSystem.h"
#include "Skill/Sub/TurretAmmo.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Enemy/Enemy_CharacterBase.h"

AArcaneTurret::AArcaneTurret()
{   
    TurretCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TurretCapsule"));
    TurretRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretRoot"));
    RotateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateMesh"));
    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
     
    SetRootComponent(TurretCapsule);
    TurretRoot->SetupAttachment(TurretCapsule);
    RotateMesh->SetupAttachment(TurretRoot);
    ProjectileSpawnPoint->SetupAttachment(RotateMesh);

    SkillCategory = ESkillCategory::Spawn_Static;
}

void AArcaneTurret::BeginPlay()
{
    Super::BeginPlay();
    SetActorLocation(GetActorLocation() + FVector(0.0f,0.0f,TurretCapsule->GetScaledCapsuleHalfHeight()));

    Enemy = FindEnemy();
    SetActorTickEnabled(true);
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AArcaneTurret::Fire, FireRate, true);
}

float AArcaneTurret::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
    if(Cast<AArcanePunkPlayerController>(EventInstigator)) return DamageApplied;

	DamageApplied = FMath::Min(HP, DamageApplied);
	
	HP = HP - DamageApplied;
	UE_LOG(LogTemp, Display, TEXT("Turret HP : %f"), HP);

    SpawnDamageText(EventInstigator, DamageAmount);

    if(HP <= 0.001f)
	{
	 	if(DeadEffect) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeadEffect, GetActorLocation(), GetActorRotation(), DeadEffectSize);
        Destroy();
	}

    return DamageApplied;
}

void AArcaneTurret::Fire()
{
    if(!Enemy) return;

    FVector ProjectileLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator ProjectileRotation = ProjectileSpawnPoint->GetComponentRotation();

	auto ProjectileActor = GetWorld()->SpawnActor<ATurretAmmo>(ProjectileClass, ProjectileLocation, ProjectileRotation);
	ProjectileActor->SetOwner(GetOwner());
}

void AArcaneTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(Enemy)
    {
        AutoAttack();
    }
    else
    {
        Enemy = FindEnemy();
    }
}

void AArcaneTurret::AutoAttack()
{
    FVector ToTarget = Enemy->GetActorLocation() - RotateMesh->GetComponentLocation();
	FRotator TargetRotation = FRotator(0, ToTarget.Rotation().Yaw, 0);

	RotateMesh->SetWorldRotation(FMath::RInterpTo(RotateMesh->GetComponentRotation(), TargetRotation, UGameplayStatics::GetWorldDeltaSeconds(this), RotateSpeed));
}

AActor* AArcaneTurret::FindEnemy()
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

void AArcaneTurret::SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility)
{
    Super::SetSkill(SkillType, SkillAbility);
}


void AArcaneTurret::SpawnDamageText(AController *EventInstigator, float Damage)
{
    ADamageText* DamageText = GetWorld()->SpawnActor<ADamageText>(DamageTextClass, GetActorLocation(), FRotator(0.0f, 180.0f, 0.0f)); if(!DamageText) return;
    DamageText->SetOwner(this);

    OwnerCharacter = Cast<AArcanePunkCharacter>(EventInstigator->GetPawn()); 
    auto InstigatorEnemy =  Cast<AEnemy_CharacterBase>(Enemy);

    if(OwnerCharacter.IsValid()) DamageText->SetDamageText(Damage, OwnerCharacter->IsCriticalAttack());
    else if(InstigatorEnemy) DamageText->SetDamageText(Damage, InstigatorEnemy->IsCriticalAttack());
	
}

