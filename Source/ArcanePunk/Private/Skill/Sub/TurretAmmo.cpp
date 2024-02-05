
#include "Skill/Sub/TurretAmmo.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "Particles/ParticleSystem.h"
#include "Components/Character/APHitPointComponent.h"
#include "Particles/ParticleSystemComponent.h"

ATurretAmmo::ATurretAmmo()
{
	PrimaryActorTick.bCanEverTick = false;

    AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
    AmmoEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AmmoEffect"));

    SetRootComponent(AmmoMesh);
    AmmoEffect->SetupAttachment(AmmoMesh);

	HitPointComp = CreateDefaultSubobject<UAPHitPointComponent>(TEXT("HitPointComp"));

	AmmoMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("AmmoMoveComp"));
	AmmoMoveComp->MaxSpeed = AmmoSpeed;
	AmmoMoveComp->InitialSpeed = AmmoSpeed;
}

// Called when the game starts or when spawned
void ATurretAmmo::BeginPlay()
{
	Super::BeginPlay();
	
	BindHit();
}

// Called every frame
void ATurretAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurretAmmo::BindHit()
{
	AmmoMesh->OnComponentHit.AddDynamic(this, &ATurretAmmo::OnHitting);
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ATurretAmmo::DestroyAmmo, DeadTime, false);
}

void ATurretAmmo::DestroyAmmo()
{
	if(HitEffet) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffet, GetActorLocation(), GetActorRotation(), FVector(0.5f, 0.5f, 0.5f));
	Destroy();
}

void ATurretAmmo::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	auto Character = Cast<AArcanePunkCharacter>(MyOwner);
	if (OtherActor && OtherActor != this && OtherActor != MyOwner && Character)
	{
		HitPointComp->DistinctHitPoint(Hit.Location, OtherActor);
		UGameplayStatics::ApplyDamage(OtherActor, Character->GetFinalATK() * DamageCoefficient, MyOwnerInstigator, this, DamageTypeClass);
        if(HitEffet) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffet, GetActorLocation(), GetActorRotation(), FVector(0.5f, 0.5f, 0.5f));
	}
    Destroy();
}


