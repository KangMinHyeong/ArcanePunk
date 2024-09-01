
#include "GameElements/Prop/APDestructibleMeshBase.h"

#include "Components/Common/APDestructibleMeshComponent.h"
#include "Components/Common/APManaDropComponent.h"
#include "Engine/DamageEvents.h"

AAPDestructibleMeshBase::AAPDestructibleMeshBase()
{
 	PrimaryActorTick.bCanEverTick = false;

	DestructibleMesh = CreateDefaultSubobject<UAPDestructibleMeshComponent>(TEXT("DestructibleMesh"));
	APManaDropComp = CreateDefaultSubobject<UAPManaDropComponent>(TEXT("APManaDropComp"));

	SetRootComponent(DestructibleMesh);
}

void AAPDestructibleMeshBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(false);
}

void AAPDestructibleMeshBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AAPDestructibleMeshBase::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
	if(HP <= KINDA_SMALL_NUMBER) return 0.0f;

	FVector HitLocation = GetActorLocation();

	if(!bCenterExplosion)
	{
		float ActualDamage = DamageAmount;
		UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
		if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
		{
			FPointDamageEvent* const PointDamageEvent = (FPointDamageEvent*) &DamageEvent;
			ActualDamage = InternalTakePointDamage(ActualDamage, *PointDamageEvent, EventInstigator, DamageCauser);

			if (ActualDamage != 0.f)
			{
				ReceivePointDamage(ActualDamage, DamageTypeCDO, PointDamageEvent->HitInfo.ImpactPoint, PointDamageEvent->HitInfo.ImpactNormal, PointDamageEvent->HitInfo.Component.Get(), PointDamageEvent->HitInfo.BoneName, PointDamageEvent->ShotDirection, EventInstigator, DamageCauser, PointDamageEvent->HitInfo);
				OnTakePointDamage.Broadcast(this, ActualDamage, EventInstigator, PointDamageEvent->HitInfo.ImpactPoint, PointDamageEvent->HitInfo.Component.Get(), PointDamageEvent->HitInfo.BoneName, PointDamageEvent->ShotDirection, DamageTypeCDO, DamageCauser);
				HitLocation = PointDamageEvent->HitInfo.ImpactPoint;
			}
		}
	}
	
	DestructibleMesh->Explode(HitLocation);
	APManaDropComp->SpawnManaDrop(DamageCauser);
	GetWorldTimerManager().SetTimer(TimerHandle, DestructibleMesh, &UAPDestructibleMeshComponent::StartFadeOut, FadeOutTime, false);

	HP = 0.0f;
    return DamageApplied;
}
