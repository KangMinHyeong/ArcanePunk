
#include "Enemy/SkillActor/APEnemyAmmo.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Engine/DamageEvents.h"

AAPEnemyAmmo::AAPEnemyAmmo()
{
 	PrimaryActorTick.bCanEverTick = false;

	AmmoRoot = CreateDefaultSubobject<USphereComponent>(TEXT("AmmoRoot"));
	AmmoEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AmmoEffect"));
	AmmoMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("AmmoMoveComp"));

	SetRootComponent(AmmoRoot);
	AmmoEffect->SetupAttachment(AmmoRoot);

	AmmoMoveComp->InitialSpeed = AmmoSpeed;
	AmmoMoveComp->MaxSpeed = AmmoSpeed;
}

void AAPEnemyAmmo::BeginPlay()
{
	Super::BeginPlay();
	
	AmmoRoot->OnComponentHit.AddDynamic(this, &AAPEnemyAmmo::OnHitting);

}

void AAPEnemyAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAPEnemyAmmo::SetOwnerEnemy(float Dist, float Radius)
{
	OwnerEnemy = Cast<AEnemy_CharacterBase>(GetOwner());

	AmmoRoot->SetSphereRadius(Radius, false);
	float DestroyTime = Dist / AmmoSpeed;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AAPEnemyAmmo::DestroyExplosion, DestroyTime, false);
}

void AAPEnemyAmmo::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	if(!OwnerEnemy.IsValid()) return; 
	float Damage = OwnerEnemy->GetCurrentATK() * OwnerEnemy->CriticalCalculate();

	FVector HitVector;	
	FPointDamageEvent myDamageEvent(Damage, Hit, HitVector, nullptr);
	AController* MyController = Cast<AController>(OwnerEnemy->GetController());
	if(MyController == nullptr) return;

	OwnerEnemy->DistinctHitPoint(Hit.Location, OtherActor);
	OtherActor->TakeDamage(Damage, myDamageEvent, MyController, this);
		
	DestroyExplosion();
}

void AAPEnemyAmmo::DestroyExplosion()
{
	auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DestroyEffect, GetActorLocation(), GetActorRotation(), ExplosionScale);
	Destroy();
}
