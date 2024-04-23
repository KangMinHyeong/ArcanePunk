
#include "Skill/ArcaneBomb.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

AArcaneBomb::AArcaneBomb()
{
    BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
    BombTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("BombTrigger"));

    SetRootComponent(BombMesh);
    BombTrigger->SetupAttachment(BombMesh);

    BombMesh->SetSimulatePhysics(true);
	BombMesh->SetMassOverrideInKg(NAME_None,1.0f, true);
	BombMesh->BodyInstance.bLockXRotation = true;
	BombMesh->BodyInstance.bLockYRotation = true;
    BombMesh->BodyInstance.bLockZRotation = true;

    SkillCategory = ESkillCategory::Throw;
}

void AArcaneBomb::BeginPlay()
{
    Super::BeginPlay();
    
}

void AArcaneBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector Target = TargetPoint - GetActorLocation();
    Target = Target/Target.Size();
    BombMesh->AddForce(Target * ThrowSpeed, TEXT("BombMesh"), false);
    
}

void AArcaneBomb::InitTrigger()
{
    BombTrigger->OnComponentBeginOverlap.AddDynamic(this, &AArcaneBomb::Explosion);
}

void AArcaneBomb::ThrowBomb()
{
    BombMesh->AddImpulse(GetOwner()->GetActorUpVector() * InitSpeed, TEXT("BombMesh"), false);
}

void AArcaneBomb::ActiveExplosion()
{
    float Size = GetActorScale3D().Y / DefaultSize;
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), Size * ExploisionScale);
}

void AArcaneBomb::Explosion(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	}
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	OwnerCharacter  = Cast<AArcanePunkCharacter>(MyOwner); if(!OwnerCharacter.IsValid()) return;
	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
        ActiveExplosion();
        OwnerCharacter->GetAttackComponent()->MultiAttack(GetActorLocation(), GetActorLocation() - FVector(0,0,-20.0f), ExplosionRadius, DamageCoefficient, 1, true, StateTime );
		Destroy();
        // if(bStun) HitPointComp->SetCrowdControl(OtherActor, ECharacterState::Stun, StateTime);
		// HitPointComp->DistinctHitPoint(SweepResult.Location, OtherActor);
		// UGameplayStatics::ApplyDamage(OtherActor, Character->GetFinalATK() * DamageCoefficient, MyOwnerInstigator, this, DamageTypeClass);
	}
    // FLatentActionInfo LatentActionInfo;
    // UKismetSystemLibrary::Delay(this, 0.2f, LatentActionInfo); Destroy();
}

void AArcaneBomb::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData)
{
    Super::SetSkill(SkillAbilityNestingData);
    // SkillAbilityComp->TraceSpeed = ThrowSpeed;
    // SkillAbilityComp->SetSkillAbility(SkillAbility, SkillCategory);
    // SkillAbilityComp->TraceSpeed = SkillAbilityComp->TraceSpeed * 0.55f;
    // SkillAbilityComp->HomingTime = 0.01f;
    // SkillAbilityComp->OwnerProjectileMove->Friction = 0.05f;
    BombTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);

}

void AArcaneBomb::SetTargetPoint(float Radius, FVector Target)
{
    float Size = GetActorScale3D().Y / DefaultSize;

    ExplosionRadius = Radius * Size;
    TargetPoint = Target;

    InitSpeed = InitSpeed * FMath::Sqrt(FMath::Sqrt(Size));

    ThrowBomb();
    InitTrigger();
    SetActorTickEnabled(true);
}
