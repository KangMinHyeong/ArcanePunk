
#include "GameElements/Projectile/APProjectileBase.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "GameElements/Trap/APTrapBase.h"

AAPProjectileBase::AAPProjectileBase()
{
 	PrimaryActorTick.bCanEverTick = true;

	AmmoRoot = CreateDefaultSubobject<USphereComponent>(TEXT("AmmoRoot"));
	AmmoMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AmmoMesh"));
	AmmoEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AmmoEffect"));
	AmmoMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("AmmoMoveComp"));

	SetRootComponent(AmmoRoot);
	AmmoMesh->SetupAttachment(AmmoRoot);
	AmmoEffect->SetupAttachment(AmmoRoot);
}

void AAPProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	AmmoRoot->OnComponentHit.AddDynamic(this, &AAPProjectileBase::OnHitting);	
	AmmoMesh->OnComponentHit.AddDynamic(this, &AAPProjectileBase::OnHitting);	
}

void AAPProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAPProjectileBase::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	auto Trap = Cast<AAPTrapBase>(GetOwner());
	if(Trap && !Trap->IsActivate()) return;

	FPointDamageEvent myDamageEvent(AmmoDamage, Hit, Hit.ImpactPoint, nullptr);
	AController* MyController = GetOwner()->GetInstigatorController();
	
	OtherActor->TakeDamage(AmmoDamage, myDamageEvent, MyController, GetOwner());
	DestroyExplosion();
}

void AAPProjectileBase::SetDestroy(float DestroyTime)
{
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AAPProjectileBase::DestroyExplosion, DestroyTime, false);
}

void AAPProjectileBase::SetSpeed(float Speed)
{
	AmmoMoveComp->InitialSpeed = Speed;
	AmmoMoveComp->MaxSpeed = Speed;
}

void AAPProjectileBase::SetRadius(float Radius)
{
	AmmoRoot->SetSphereRadius(Radius);
}

void AAPProjectileBase::DestroyExplosion()
{
	auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DestroyEffect, GetActorLocation(), GetActorRotation(), ExplosionScale);
	Destroy();
}
