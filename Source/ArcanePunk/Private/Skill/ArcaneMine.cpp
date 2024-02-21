
#include "Skill/ArcaneMine.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

AArcaneMine::AArcaneMine()
{
	MineTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("MineTrigger"));
	MineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MineMesh"));

    SetRootComponent(MineTrigger);
    MineMesh->SetupAttachment(MineTrigger);
	
    MineMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MineMoveComp"));
	MineMoveComp->MaxSpeed = MineSpeed;
	MineMoveComp->InitialSpeed = MineSpeed;

    SkillCategory = ESkillCategory::Projecitle;
}

void AArcaneMine::BeginPlay()
{
    Super::BeginPlay();

    BintHit();
}

void AArcaneMine::Tick(float DeltaTime)
{
}

void AArcaneMine::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	
	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
        Explosion(OtherActor);
	}
}

void AArcaneMine::Explosion(AActor* OtherActor)
{
    auto Character = Cast<AArcanePunkCharacter>(GetOwner());  
    float Size =  GetActorScale3D().Y / DefaultSize;

    Character->GetAttackComponent()->MultiAttack_KnockBack(GetActorLocation(), GetActorLocation() + Character->GetActorUpVector() * 25.0f, ExplosionRadius * Size, ExplosionRadius, DamageCoefficient, HitNumbers, StateTime, true);
    // DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius* Size, 18, FColor::Green,false, 2.0f);
    if(ExplosionEffect) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), ExplosionScale);
    Destroy();
}

void AArcaneMine::SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility)
{
	Super::SetSkill(SkillType, SkillAbility);
}


void AArcaneMine::BintHit()
{
    MineTrigger->OnComponentHit.AddDynamic(this, &AArcaneMine::OnHitting);
}